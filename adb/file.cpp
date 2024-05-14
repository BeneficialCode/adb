#include "file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#if defined(__APPLE__)

#endif
#if defined(_WIN32)
#include <WinSock2.h>
#include <direct.h>
#include <windows.h>
#define O_NOFOLLOW 0
#define OS_PATH_SEPARATOR '\\'
#else

#endif

#include "logging.h"
#include "macros.h"
#include "unique_fd.h"
#include "utf8.h"



namespace {
#ifdef _WIN32
    static int mkstemp(char* name_template, size_t size_in_chars) {
        std::wstring path;
        CHECK(android::base::UTF8ToWide(name_template, &path))
            << "path can't be converted to wchar: " << name_template;
        if (_wmktemp_s(path.data(), path.size() + 1) != 0) {
            return -1;
        }

        // Use open() to match the close() that TemporaryFile's destructor does.
        // Use O_BINARY to match base file APIs.
        int fd = -1;
        errno_t err = _wsopen_s(&fd, path.c_str(), O_CREAT | O_EXCL | O_RDWR | O_BINARY, _SH_DENYRW,
            _S_IREAD | _S_IWRITE);
        if (fd < 0) {
            return -1;
        }

        std::string path_utf8;
        CHECK(android::base::WideToUTF8(path, &path_utf8)) << "path can't be converted to utf8";
        CHECK(strcpy_s(name_template, size_in_chars, path_utf8.c_str()) == 0)
            << "utf8 path can't be assigned back to name_template";

        return fd;
    }

    static char* mkdtemp(char* name_template, size_t size_in_chars) {
        std::wstring path;
        CHECK(android::base::UTF8ToWide(name_template, &path))
            << "path can't be converted to wchar: " << name_template;

        if (_wmktemp_s(path.data(), path.size() + 1) != 0) {
            return nullptr;
        }

        if (_wmkdir(path.c_str()) != 0) {
            return nullptr;
        }

        std::string path_utf8;
        CHECK(android::base::WideToUTF8(path, &path_utf8)) << "path can't be converted to utf8";
        CHECK(strcpy_s(name_template, size_in_chars, path_utf8.c_str()) == 0)
            << "utf8 path can't be assigned back to name_template";

        return name_template;
    }
#endif

    std::string GetSystemTempDir() {
#if defined(__ANDROID__)

#elif defined(_WIN32)
        wchar_t tmp_dir_w[MAX_PATH];
        DWORD result = GetTempPathW(std::size(tmp_dir_w), tmp_dir_w);  // checks TMP env
        CHECK_NE(result, 0ul) << "GetTempPathW failed, error: " << GetLastError();
        CHECK_LT(result, std::size(tmp_dir_w)) << "path truncated to: " << result;

        // GetTempPath() returns a path with a trailing slash, but init()
        // does not expect that, so remove it.
        if (tmp_dir_w[result - 1] == L'\\') {
            tmp_dir_w[result - 1] = L'\0';
        }

        std::string tmp_dir;
        CHECK(android::base::WideToUTF8(tmp_dir_w, &tmp_dir)) << "path can't be converted to utf8";

        return tmp_dir;
#else
      
#endif
    }

    static int __basename_r(const char* path, char* buffer, size_t buffer_size) {
        const char* startp = nullptr;
        const char* endp = nullptr;
        int len;
        int result;
        // Empty or NULL string gets treated as ".".
        if (path == nullptr || *path == '\0') {
            startp = ".";
            len = 1;
            goto Exit;
        }
        // Strip trailing slashes.
        endp = path + strlen(path) - 1;
        while (endp > path && *endp == '/') {
            endp--;
        }
        // All slashes becomes "/".
        if (endp == path && *endp == '/') {
            startp = "/";
            len = 1;
            goto Exit;
        }
        // Find the start of the base.
        startp = endp;
        while (startp > path && *(startp - 1) != '/') {
            startp--;
        }
        len = endp - startp + 1;
    Exit:
        result = len;
        if (buffer == nullptr) {
            return result;
        }
        if (len > static_cast<int>(buffer_size) - 1) {
            len = buffer_size - 1;
            result = -1;
            errno = ERANGE;
        }
        if (len >= 0) {
            memcpy(buffer, startp, len);
            buffer[len] = 0;
        }
        return result;
    }
    // Since this is a non-standard symbol, it might be hijacked by a basename_r in the executable.
    int basename_r(const char* path, char* buffer, size_t buffer_size) {
        return __basename_r(path, buffer, buffer_size);
    }

    static int __dirname_r(const char* path, char* buffer, size_t buffer_size) {
        const char* endp = nullptr;
        int len;
        int result;
        // Empty or NULL string gets treated as ".".
        if (path == nullptr || *path == '\0') {
            path = ".";
            len = 1;
            goto Exit;
        }
        // Strip trailing slashes.
        endp = path + strlen(path) - 1;
        while (endp > path && *endp == '/') {
            endp--;
        }
        // Find the start of the dir.
        while (endp > path && *endp != '/') {
            endp--;
        }
        // Either the dir is "/" or there are no slashes.
        if (endp == path) {
            path = (*endp == '/') ? "/" : ".";
            len = 1;
            goto Exit;
        }
        do {
            endp--;
        } while (endp > path && *endp == '/');
        len = endp - path + 1;
    Exit:
        result = len;
        if (len + 1 > MAX_PATH) {
            errno = ENAMETOOLONG;
            return -1;
        }
        if (buffer == nullptr) {
            return result;
        }
        if (len > static_cast<int>(buffer_size) - 1) {
            len = buffer_size - 1;
            result = -1;
            errno = ERANGE;
        }
        if (len >= 0) {
            memcpy(buffer, path, len);
            buffer[len] = 0;
        }
        return result;
    }
    // Since this is a non-standard symbol, it might be hijacked by a basename_r in the executable.
    int dirname_r(const char* path, char* buffer, size_t buffer_size) {
        return __dirname_r(path, buffer, buffer_size);
    }
    char* basename(const char* path) {
        char* buf = (char*)LocalAlloc(LPTR, MAX_PATH);
        int rc = __basename_r(path, buf, MAX_PATH);
        return (rc < 0) ? nullptr : buf;
    }
    char* dirname(const char* path) {
        char* buf = (char*)LocalAlloc(LPTR, MAX_PATH);
        int rc = __dirname_r(path, buf, MAX_PATH);
        return (rc < 0) ? nullptr : buf;
    }

}  // namespace

TemporaryFile::TemporaryFile() {
    init(GetSystemTempDir());
}

TemporaryFile::TemporaryFile(const std::string& tmp_dir) {
    init(tmp_dir);
}

TemporaryFile::~TemporaryFile() {
    if (fd != -1) {
        _close(fd);
    }
    if (remove_file_) {
        _unlink(path);
    }
}

int TemporaryFile::release() {
    int result = fd;
    fd = -1;
    return result;
}

void TemporaryFile::init(const std::string& tmp_dir) {
    snprintf(path, sizeof(path), "%s%cTemporaryFile-XXXXXX", tmp_dir.c_str(), OS_PATH_SEPARATOR);
#if defined(_WIN32)
    fd = mkstemp(path, sizeof(path));
#else
    fd = mkstemp(path);
#endif
}

TemporaryDir::TemporaryDir() {
    init(GetSystemTempDir());
}

TemporaryDir::~TemporaryDir() {
    if (!remove_dir_and_contents_) return;
}

bool TemporaryDir::init(const std::string& tmp_dir) {
    snprintf(path, sizeof(path), "%s%cTemporaryDir-XXXXXX", tmp_dir.c_str(), OS_PATH_SEPARATOR);
#if defined(_WIN32)
    return (mkdtemp(path, sizeof(path)) != nullptr);
#else
    return (mkdtemp(path) != nullptr);
#endif
}

#ifndef	S_ISREG
#define	S_ISREG(x)	((x) & S_IFREG)
#endif	/* S_ISREG */

namespace android {
    namespace base {

        // Versions of standard library APIs that support UTF-8 strings.
        using namespace android::base::utf8;

        bool ReadFdToString(borrowed_fd fd, std::string* content) {
            content->clear();

            // Although original we had small files in mind, this code gets used for
            // very large files too, where the std::string growth heuristics might not
            // be suitable. https://code.google.com/p/android/issues/detail?id=258500.
            struct stat sb;
            if (fstat(fd.get(), &sb) != -1 && sb.st_size > 0) {
                content->reserve(sb.st_size);
            }

            char buf[4096];
            size_t n;
            while ((n = TEMP_FAILURE_RETRY(_read,fd.get(), &buf[0], sizeof(buf))) > 0) {
                content->append(buf, n);
            }
            return (n == 0) ? true : false;
        }

        bool ReadFileToString(const std::string& path, std::string* content, bool follow_symlinks) {
            content->clear();

            int flags = O_RDONLY | O_CLOEXEC | O_BINARY | (follow_symlinks ? 0 : O_NOFOLLOW);
            android::base::unique_fd fd(TEMP_FAILURE_RETRY(open, path.c_str(), flags));
            if (fd == -1) {
                return false;
            }
            return ReadFdToString(fd, content);
        }

        bool WriteStringToFd(std::string_view content, borrowed_fd fd) {
            const char* p = content.data();
            size_t left = content.size();
            while (left > 0) {
                size_t n = TEMP_FAILURE_RETRY(_write, fd.get(), p, left);
                if (n == -1) {
                    return false;
                }
                p += n;
                left -= n;
            }
            return true;
        }

        static bool CleanUpAfterFailedWrite(const std::string& path) {
            // Something went wrong. Let's not leave a corrupt file lying around.
            int saved_errno = errno;
            unlink(path.c_str());
            errno = saved_errno;
            return false;
        }

#if !defined(_WIN32)
        
#endif

        bool WriteStringToFile(const std::string& content, const std::string& path,
            bool follow_symlinks) {
            int flags = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_BINARY |
                (follow_symlinks ? 0 : O_NOFOLLOW);
            android::base::unique_fd fd(TEMP_FAILURE_RETRY(open, path.c_str(), flags, 0666));
            if (fd == -1) {
                return false;
            }
            return WriteStringToFd(content, fd) || CleanUpAfterFailedWrite(path);
        }

        bool ReadFully(borrowed_fd fd, void* data, size_t byte_count) {
            uint8_t* p = reinterpret_cast<uint8_t*>(data);
            size_t remaining = byte_count;
            while (remaining > 0) {
                size_t n = TEMP_FAILURE_RETRY(_read, fd.get(), p, remaining);
                if (n <= 0) return false;
                p += n;
                remaining -= n;
            }
            return true;
        }

#if defined(_WIN32)
        // Windows implementation of pread. Note that this DOES move the file descriptors read position,
        // but it does so atomically.
        static size_t pread(borrowed_fd fd, void* data, size_t byte_count, off64_t offset) {
            DWORD bytes_read;
            OVERLAPPED overlapped;
            memset(&overlapped, 0, sizeof(OVERLAPPED));
            overlapped.Offset = static_cast<DWORD>(offset);
            overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32);
            if (!ReadFile(reinterpret_cast<HANDLE>(_get_osfhandle(fd.get())), data,
                static_cast<DWORD>(byte_count), &bytes_read, &overlapped)) {
                // In case someone tries to read errno (since this is masquerading as a POSIX call)
                errno = EIO;
                return -1;
            }
            return static_cast<size_t>(bytes_read);
        }

        static size_t pwrite(borrowed_fd fd, const void* data, size_t byte_count, off64_t offset) {
            DWORD bytes_written;
            OVERLAPPED overlapped;
            memset(&overlapped, 0, sizeof(OVERLAPPED));
            overlapped.Offset = static_cast<DWORD>(offset);
            overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32);
            if (!WriteFile(reinterpret_cast<HANDLE>(_get_osfhandle(fd.get())), data,
                static_cast<DWORD>(byte_count), &bytes_written, &overlapped)) {
                // In case someone tries to read errno (since this is masquerading as a POSIX call)
                errno = EIO;
                return -1;
            }
            return static_cast<size_t>(bytes_written);
        }
#endif

        bool ReadFullyAtOffset(borrowed_fd fd, void* data, size_t byte_count, off64_t offset) {
            uint8_t* p = reinterpret_cast<uint8_t*>(data);
            while (byte_count > 0) {
                size_t n = TEMP_FAILURE_RETRY(pread, fd.get(), p, byte_count, offset);
                if (n <= 0) return false;
                p += n;
                byte_count -= n;
                offset += n;
            }
            return true;
        }

        bool WriteFullyAtOffset(borrowed_fd fd, const void* data, size_t byte_count, off64_t offset) {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
            size_t remaining = byte_count;
            while (remaining > 0) {
                size_t n = TEMP_FAILURE_RETRY(pwrite, fd.get(), p, remaining, offset);
                if (n == -1) return false;
                p += n;
                remaining -= n;
                offset += n;
            }
            return true;
        }

        bool WriteFully(borrowed_fd fd, const void* data, size_t byte_count) {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
            size_t remaining = byte_count;
            while (remaining > 0) {
                size_t n = TEMP_FAILURE_RETRY(_write, fd.get(), p, remaining);
                if (n == -1) return false;
                p += n;
                remaining -= n;
            }
            return true;
        }

        bool RemoveFileIfExists(const std::string& path, std::string* err) {
            struct stat st;
#if defined(_WIN32)
            // TODO: Windows version can't handle symbolic links correctly.
            int result = stat(path.c_str(), &st);
            bool file_type_removable = (result == 0 && S_ISREG(st.st_mode));
#else
            
#endif
            if (result == -1) {
                if (errno == ENOENT || errno == ENOTDIR) return true;
                if (err != nullptr) { 
                    char error[256];
                    strerror_s(error,sizeof(error),errno);
                    *err = error;
                }
                return false;
            }

            if (result == 0) {
                if (!file_type_removable) {
                    if (err != nullptr) {
                        *err = "is not a regular file or symbolic link";
                    }
                    return false;
                }
                if (unlink(path.c_str()) == -1) {
                    if (err != nullptr) {
                        char error[256];
                        strerror_s(error,sizeof(error),errno);
                        *err = error;
                    }
                    return false;
                }
            }
            return true;
        }

#if !defined(_WIN32)
       
#endif

#if !defined(_WIN32)
        
#endif

        std::string GetExecutablePath() {
#if defined(__linux__)

#elif defined(__APPLE__)
          
#elif defined(_WIN32)
            char path[MAX_PATH + 1];
            DWORD result = GetModuleFileNameA(NULL, path, sizeof(path) - 1);
            if (result == 0 || result == sizeof(path) - 1) return "";
            path[MAX_PATH - 1] = 0;
            return path;
#elif defined(__EMSCRIPTEN__)
            abort();
#else
#error unknown OS
#endif
        }

        std::string GetExecutableDirectory() {
            return Dirname(GetExecutablePath());
        }

#if defined(_WIN32)
        std::string Basename(std::string_view path) {
            // TODO: how much of this is actually necessary for mingw?

            // Copy path because basename may modify the string passed in.
            std::string result(path);

            // Use lock because basename() may write to a process global and return a
            // pointer to that. Note that this locking strategy only works if all other
            // callers to basename in the process also grab this same lock, but its
            // better than nothing.  Bionic's basename returns a thread-local buffer.
            static std::mutex& basename_lock = *new std::mutex();
            std::lock_guard<std::mutex> lock(basename_lock);

            // Note that if std::string uses copy-on-write strings, &str[0] will cause
            // the copy to be made, so there is no chance of us accidentally writing to
            // the storage for 'path'.
            char* name = basename(&result[0]);

            // In case basename returned a pointer to a process global, copy that string
            // before leaving the lock.
            result.assign(name);

            return result;
        }
#else
       
#endif

#if defined(_WIN32)
        std::string Dirname(std::string_view path) {
            // TODO: how much of this is actually necessary for mingw?

            // Copy path because dirname may modify the string passed in.
            std::string result(path);

            // Use lock because dirname() may write to a process global and return a
            // pointer to that. Note that this locking strategy only works if all other
            // callers to dirname in the process also grab this same lock, but its
            // better than nothing.  Bionic's dirname returns a thread-local buffer.
            static std::mutex& dirname_lock = *new std::mutex();
            std::lock_guard<std::mutex> lock(dirname_lock);

            // Note that if std::string uses copy-on-write strings, &str[0] will cause
            // the copy to be made, so there is no chance of us accidentally writing to
            // the storage for 'path'.
            char* parent = dirname(&result[0]);

            // In case dirname returned a pointer to a process global, copy that string
            // before leaving the lock.
            result.assign(parent);

            return result;
        }
#else
       
#endif

    }  // namespace base
}  // namespace android