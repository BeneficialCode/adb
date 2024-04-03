#pragma once
#include <errno.h>

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "macros.h"
#include "off64_t.h"
#include "unique_fd.h"
#include "utf8.h"

#include "adb_unique_fd.h"
#include "network.h"
#include "stat.h"

#include <ctype.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <stdint.h>
#include <sys/stat.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <windows.h>


#include <memory>   // unique_ptr
#include <string>

#include "dirent-win32.h"


#define OS_PATH_SEPARATORS "\\/"
#define OS_PATH_SEPARATOR '\\'
#define OS_PATH_SEPARATOR_STR "\\"
#define ENV_PATH_SEPARATOR_STR ";"

static inline bool adb_is_separator(char c) {
	return c == '\\' || c == '/';
}

extern int adb_thread_setname(const std::string& name);

static inline void close_on_exec(borrowed_fd fd) {
	/* nothing really */
}

extern int adb_unlink(const char* path);
#undef unlink
#define unlink ___xxx_unlink

extern int adb_mkdir(const std::string& path, int mode);
#undef mkdir
#define mkdir ___xxx_mkdir

extern int adb_rename(const char* oldpath, const char* newpath);

// See the comments for the !defined(_WIN32) versions of adb_*().
extern int adb_open(const char* path, int options);
extern int adb_creat(const char* path, int mode);
extern int adb_read(borrowed_fd fd, void* buf, int len);
extern int adb_pread(borrowed_fd fd, void* buf, int len, off64_t offset);
extern int adb_write(borrowed_fd fd, const void* buf, int len);
extern int adb_pwrite(borrowed_fd fd, const void* buf, int len, off64_t offset);
extern int64_t adb_lseek(borrowed_fd fd, int64_t pos, int where);
extern int adb_shutdown(borrowed_fd fd, int direction = SD_BOTH); // shutdown both send and receive by default
extern int adb_close(int fd);
extern int adb_register_socket(SOCKET s);
extern HANDLE adb_get_os_handle(borrowed_fd fd);

extern int adb_gethostname(char* name, size_t len);
extern int adb_getlogin_r(char* buf, size_t bufsize);

// See the comments for the !defined(_WIN32) version of unix_close().
static inline int unix_close(int fd) {
	return _close(fd);
}
#undef close
#define close ____xxx_close

// Like unix_read(), but may return EINTR.
extern int unix_read_interruptible(borrowed_fd fd, void* buf, size_t len);

// See the comments for the !defined(_WIN32) version of unix_read().
static inline int unix_read(borrowed_fd fd, void* buf, size_t len) {
	return TEMP_FAILURE_RETRY(unix_read_interruptible, fd, buf, len);
}

#undef   read
#define  read  ___xxx_read

#undef pread
#define pread ___xxx_pread

// See the comments for the !defined(_WIN32) version of unix_write().
static inline int unix_write(borrowed_fd fd, const void* buf, size_t len) {
	return _write(fd.get(), buf, len);
}
#undef   write
#define  write  ___xxx_write

#undef pwrite
#define pwrite ___xxx_pwrite

// See the comments for the !defined(_WIN32) version of unix_lseek().
static inline int unix_lseek(borrowed_fd fd, int pos, int where) {
	return _lseek(fd.get(), pos, where);
}
#undef lseek
#define lseek ___xxx_lseek

// See the comments for the !defined(_WIN32) version of adb_open_mode().
static inline int adb_open_mode(const char* path, int options, int mode) {
	return adb_open(path, options);
}

// See the comments for the !defined(_WIN32) version of unix_open().
extern int unix_open(std::string_view path, int options, ...);
#define  open    ___xxx_unix_open

// Checks if |fd| corresponds to a console.
// Standard Windows isatty() returns 1 for both console FDs and character
// devices like NUL. unix_isatty() performs some extra checking to only match
// console FDs.
// |fd| must be a real file descriptor, meaning STDxx_FILENO or unix_open() FDs
// will work but adb_open() FDs will not. Additionally the OS handle associated
// with |fd| must have GENERIC_READ access (which console FDs have by default).
// Returns 1 if |fd| is a console FD, 0 otherwise. The value of errno after
// calling this function is unreliable and should not be used.
int unix_isatty(borrowed_fd fd);
#define  isatty  ___xxx_isatty

int network_inaddr_any_server(int port, int type, std::string* error);

inline int network_local_client(const char* name, int namespace_id, int type, std::string* error) {
	abort();
}

inline int network_local_server(const char* name, int namespace_id, int type, std::string* error) {
	abort();
}

int network_connect(const std::string& host, int port, int type, int timeout,
	std::string* error);

std::optional<size_t> network_peek(borrowed_fd fd);

extern int adb_socket_accept(borrowed_fd serverfd, struct sockaddr* addr, socklen_t* addrlen);

#undef   accept
#define  accept  ___xxx_accept

int adb_getsockname(borrowed_fd fd, struct sockaddr* sockaddr, socklen_t* addrlen);

// Returns the local port number of a bound socket, or -1 on failure.
int adb_socket_get_local_port(borrowed_fd fd);

extern int adb_setsockopt(borrowed_fd fd, int level, int optname, const void* optval,
	socklen_t optlen);

#undef   setsockopt
#define  setsockopt  ___xxx_setsockopt

// Wrapper around socket() call. On Windows, ADB has an indirection layer for file descriptors.
extern int adb_socket(int domain, int type, int protocol);

// Wrapper around bind() call, as Windows has indirection layer.
extern int adb_bind(borrowed_fd fd, const sockaddr* addr, int namelen);

extern int adb_socketpair(int sv[2]);

// Posix compatibility layer for msghdr
struct adb_msghdr {
	void* msg_name;
	socklen_t msg_namelen;
	struct adb_iovec* msg_iov;
	size_t msg_iovlen;
	void* msg_control;
	size_t msg_controllen;
	int msg_flags;
};

size_t adb_sendmsg(borrowed_fd fd, const adb_msghdr* msg, int flags);
size_t adb_recvmsg(borrowed_fd fd, adb_msghdr* msg, int flags);

using adb_cmsghdr = WSACMSGHDR;

extern adb_cmsghdr* adb_CMSG_FIRSTHDR(adb_msghdr* msgh);
extern adb_cmsghdr* adb_CMSG_NXTHDR(adb_msghdr* msgh, adb_cmsghdr* cmsg);
extern unsigned char* adb_CMSG_DATA(adb_cmsghdr* cmsg);

struct adb_pollfd {
	int fd;
	short events;
	short revents;
};
extern int adb_poll(adb_pollfd* fds, size_t nfds, int timeout);
#define poll ___xxx_poll

static inline int adb_is_absolute_host_path(const char* path) {
	return isalpha(path[0]) && path[1] == ':' && path[2] == '\\';
}

// UTF-8 versions of POSIX APIs.
extern DIR* adb_opendir(const char* dirname);
extern struct dirent* adb_readdir(DIR* dir);
extern int adb_closedir(DIR* dir);

extern int adb_utime(const char*, struct utimbuf*);
extern int adb_chmod(const char*, int);

extern int adb_vfprintf(FILE* stream, const char* format, va_list ap);
extern int adb_vprintf(const char* format, va_list ap);
extern int adb_fprintf(FILE* stream, const char* format, ...);
extern int adb_printf(const char* format, ...);
extern int adb_fputs(const char* buf, FILE* stream);
extern int adb_fputc(int ch, FILE* stream);
extern int adb_putchar(int ch);
extern int adb_puts(const char* buf);
extern size_t adb_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

extern FILE* adb_fopen(const char* f, const char* m);

extern char* adb_getenv(const char* name);

extern char* adb_getcwd(char* buf, int size);

// Remap calls to POSIX APIs to our UTF-8 versions.
#define opendir adb_opendir
#define readdir adb_readdir
#define closedir adb_closedir
#define rewinddir rewinddir_utf8_not_yet_implemented
#define telldir telldir_utf8_not_yet_implemented
// Some compiler's C++ headers have members named seekdir, so we can't do the
// macro technique and instead cause a link error if seekdir is called.
inline void seekdir(DIR*, long) {
	extern int seekdir_utf8_not_yet_implemented;
	seekdir_utf8_not_yet_implemented = 1;
}

#define chmod adb_chmod

#define vfprintf adb_vfprintf
#define vprintf adb_vprintf
#define fprintf adb_fprintf
#define printf adb_printf
#define fputs adb_fputs
#define fputc adb_fputc
// putc may be a macro, so if so, undefine it, so that we can redefine it.
#undef putc
#define putc(c, s) adb_fputc(c, s)
#define putchar adb_putchar
#define puts adb_puts
#define fwrite adb_fwrite

#define fopen adb_fopen
#define freopen freopen_utf8_not_yet_implemented

#define getenv adb_getenv
#define putenv putenv_utf8_not_yet_implemented
#define setenv setenv_utf8_not_yet_implemented
#define unsetenv unsetenv_utf8_not_yet_implemented

#define getcwd adb_getcwd

// A very simple wrapper over a launched child process
class Process {
public:
	constexpr explicit Process(HANDLE h = nullptr) : h_(h) {}
	constexpr Process(Process&& other) : h_(std::exchange(other.h_, nullptr)) {}
	~Process() { close(); }
	constexpr explicit operator bool() const { return h_ != nullptr; }

	void wait() {
		if (*this) {
			::WaitForSingleObject(h_, INFINITE);
			close();
		}
	}
	void kill() {
		if (*this) {
			::TerminateProcess(h_, -1);
		}
	}

private:
	DISALLOW_COPY_AND_ASSIGN(Process);

	void close() {
		if (*this) {
			::CloseHandle(h_);
			h_ = nullptr;
		}
	}

	HANDLE h_;
};

Process adb_launch_process(std::string_view executable, std::vector<std::string> args,
	std::initializer_list<int> fds_to_inherit = {});

// Helper class to convert UTF-16 argv from wmain() to UTF-8 args that can be
// passed to main().
class NarrowArgs {
public:
	NarrowArgs(int argc, wchar_t** argv);
	~NarrowArgs();

	inline char** data() {
		return narrow_args;
	}

private:
	char** narrow_args;
};

// Windows HANDLE values only use 32-bits of the type, even on 64-bit machines,
// so they can fit in an int. To convert back, we just need to sign-extend.
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa384203%28v=vs.85%29.aspx
// Note that this does not make a HANDLE value work with APIs like open(), nor
// does this make a value from open() passable to APIs taking a HANDLE. This
// just lets you take a HANDLE, pass it around as an int, and then use it again
// as a HANDLE.
inline int cast_handle_to_int(const HANDLE h) {
	// truncate
	return static_cast<int>(reinterpret_cast<INT_PTR>(h));
}

inline HANDLE cast_int_to_handle(const int fd) {
	// sign-extend
	return reinterpret_cast<HANDLE>(static_cast<INT_PTR>(fd));
}

// Deleter for unique_handle. Adapted from many sources, including:
// http://stackoverflow.com/questions/14841396/stdunique-ptr-deleters-and-the-win32-api
// https://visualstudiomagazine.com/articles/2013/09/01/get-a-handle-on-the-windows-api.aspx
class handle_deleter {
public:
	typedef HANDLE pointer;

	void operator()(HANDLE h);
};

// Like std::unique_ptr, but for Windows HANDLE objects that should be
// CloseHandle()'d. Operator bool() only checks if the handle != nullptr,
// but does not check if the handle != INVALID_HANDLE_VALUE.
typedef std::unique_ptr<HANDLE, handle_deleter> unique_handle;

namespace internal {

	size_t ParseCompleteUTF8(const char* first, const char* last, std::vector<char>* remaining_bytes);

}


static inline void disable_tcp_nagle(borrowed_fd fd) {
	int off = 1;
	adb_setsockopt(fd.get(), IPPROTO_TCP, TCP_NODELAY, &off, sizeof(off));
}

// Sets TCP socket |fd| to send a keepalive TCP message every |interval_sec| seconds. Set
// |interval_sec| to 0 to disable keepalives. If keepalives are enabled, the connection will be
// configured to drop after 10 missed keepalives. Returns true on success.
bool set_tcp_keepalive(borrowed_fd fd, int interval_sec);

// Returns a human-readable OS version string.
extern std::string GetOSVersion(void);

#if defined(_WIN32)
// Win32 defines ERROR, which we don't need, but which conflicts with google3 logging.
#undef ERROR
#endif