#include "logging.h"
#include "strings.h"



#include <fcntl.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>

#include <iostream>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int basename_r(const char* path, char* buffer, size_t  bufflen)
{
    const char* endp, * startp;
    int         len, result;
    char        temp[2];
    /* Empty or NULL string gets treated as "." */
    if (path == NULL || *path == '\0') {
        startp = ".";
        len = 1;
        goto Exit;
    }
    /* Strip trailing slashes */
    endp = path + strlen(path) - 1;
    while (endp > path && *endp == '/')
        endp--;
    /* All slashes becomes "/" */
    if (endp == path && *endp == '/') {
        startp = "/";
        len = 1;
        goto Exit;
    }
    /* Find the start of the base */
    startp = endp;
    while (startp > path && *(startp - 1) != '/')
        startp--;
    len = endp - startp + 1;
Exit:
    result = len;
    if (buffer == NULL) {
        return result;
    }
    if (len > (int)bufflen - 1) {
        len = (int)bufflen - 1;
        result = -1;
        errno = ERANGE;
    }
    if (len >= 0) {
        memcpy(buffer, startp, len);
        buffer[len] = 0;
    }
    return result;
}

char* basename(const char* path)
{
    static char* bname = NULL;
    int           ret;
    if (bname == NULL) {
        bname = (char*)malloc(MAX_PATH);
        if (bname == NULL)
            return(NULL);
    }
    ret = basename_r(path, bname, MAX_PATH);
    return (ret < 0) ? NULL : bname;
}

const char* getprogname() {
    static bool first = true;
    static char progname[MAX_PATH] = {};
    if (first) {
        CHAR longname[MAX_PATH];
        DWORD nchars = GetModuleFileNameA(nullptr, longname, arraysize(longname));
        if ((nchars >= arraysize(longname)) || (nchars == 0)) {
            // String truncation or some other error.
            strcpy_s(progname, "<unknown>");
        }
        else {
            strcpy_s(progname, basename(longname));
        }
        first = false;
    }
    return progname;
}

namespace android {
    namespace base {
        static std::mutex& LoggingLock() {
            static auto& logging_lock = *new std::mutex();
            return logging_lock;
        }
        static LogFunction& Logger() {
#ifdef __ANDROID__
            static auto& logger = *new LogFunction(LogdLogger());
#else
            static auto& logger = *new LogFunction(StderrLogger);
#endif
            return logger;
        }
        static AbortFunction& Aborter() {
            static auto& aborter = *new AbortFunction(DefaultAborter);
            return aborter;
        }
        static std::recursive_mutex& TagLock() {
            static auto& tag_lock = *new std::recursive_mutex();
            return tag_lock;
        }
        static std::string* gDefaultTag;
        std::string GetDefaultTag() {
            std::lock_guard<std::recursive_mutex> lock(TagLock());
            if (gDefaultTag == nullptr) {
                return "";
            }
            return *gDefaultTag;
        }
        void SetDefaultTag(const std::string& tag) {
            std::lock_guard<std::recursive_mutex> lock(TagLock());
            if (gDefaultTag != nullptr) {
                delete gDefaultTag;
                gDefaultTag = nullptr;
            }
            if (!tag.empty()) {
                gDefaultTag = new std::string(tag);
            }
        }
        static bool gInitialized = false;
        static LogSeverity gMinimumLogSeverity = INFO;
#if defined(__linux__)
        
#endif
        void StderrLogger(LogId, LogSeverity severity, const char* tag, const char* file, unsigned int line,
            const char* message) {
            struct tm now;
            time_t t = time(nullptr);
#if defined(_WIN32)
            localtime_s(&now, &t);
#else
            localtime_r(&t, &now);
#endif
            char timestamp[32];
            strftime(timestamp, sizeof(timestamp), "%m-%d %H:%M:%S", &now);
            static const char log_characters[] = "VDIWEFF";
            static_assert(arraysize(log_characters) - 1 == FATAL + 1,
                "Mismatch in size of log_characters and values in LogSeverity");
            char severity_char = log_characters[severity];
        }
        void StdioLogger(LogId, LogSeverity severity, const char* /*tag*/, const char* /*file*/,
            unsigned int /*line*/, const char* message) {
            if (severity >= WARNING) {
                fflush(stdout);
                fprintf(stderr, "%s: %s\n", getprogname(), message);
            }
            else {
                fprintf(stdout, "%s\n", message);
            }
        }
        void DefaultAborter(const char* abort_message) {
#ifdef __ANDROID__
            
#else
            
#endif
            abort();
        }
#ifdef __ANDROID__
        
#endif
        void InitLogging(char* argv[], LogFunction&& logger, AbortFunction&& aborter) {
            SetLogger(std::forward<LogFunction>(logger));
            SetAborter(std::forward<AbortFunction>(aborter));
            if (gInitialized) {
                return;
            }
            gInitialized = true;
            // Stash the command line for later use. We can use /proc/self/cmdline on
            // Linux to recover this, but we don't have that luxury on the Mac/Windows,
            // and there are a couple of argv[0] variants that are commonly used.
            if (argv != nullptr) {
                SetDefaultTag(basename(argv[0]));
            }
            
            char* tags;
            size_t len;
            errno_t err = _dupenv_s(&tags, &len, "ANDROID_LOG_TAGS");
            if (tags == nullptr) {
                return;
            }
            std::vector<std::string> specs = Split(tags, " ");
            for (size_t i = 0; i < specs.size(); ++i) {
                // "tag-pattern:[vdiwefs]"
                std::string spec(specs[i]);
                if (spec.size() == 3 && StartsWith(spec, "*:")) {
                    switch (spec[2]) {
                    case 'v':
                        gMinimumLogSeverity = VERBOSE;
                        continue;
                    case 'd':
                        gMinimumLogSeverity = DEBUG;
                        continue;
                    case 'i':
                        gMinimumLogSeverity = INFO;
                        continue;
                    case 'w':
                        gMinimumLogSeverity = WARNING;
                        continue;
                    case 'e':
#pragma push_macro("ERROR")
#undef ERROR
                        gMinimumLogSeverity = ERROR;
#pragma pop_macro("ERROR")
                        continue;
                    case 'f':
                        gMinimumLogSeverity = FATAL_WITHOUT_ABORT;
                        continue;
                        // liblog will even suppress FATAL if you say 's' for silent, but that's
                        // crazy!
                    case 's':
                        gMinimumLogSeverity = FATAL_WITHOUT_ABORT;
                        continue;
                    }
                }
                LOG(FATAL) << "unsupported '" << spec << "' in ANDROID_LOG_TAGS (" << tags
                    << ")";
            }
        }
        void SetLogger(LogFunction&& logger) {
            std::lock_guard<std::mutex> lock(LoggingLock());
            Logger() = std::move(logger);
        }
        void SetAborter(AbortFunction&& aborter) {
            std::lock_guard<std::mutex> lock(LoggingLock());
            Aborter() = std::move(aborter);
        }
        static const char* GetFileBasename(const char* file) {
            // We can't use basename(3) even on Unix because the Mac doesn't
            // have a non-modifying basename.
            const char* last_slash = strrchr(file, '/');
            if (last_slash != nullptr) {
                return last_slash + 1;
            }
#if defined(_WIN32)
            const char* last_backslash = strrchr(file, '\\');
            if (last_backslash != nullptr) {
                return last_backslash + 1;
            }
#endif
            return file;
        }
        // This indirection greatly reduces the stack impact of having lots of
        // checks/logging in a function.
        class LogMessageData {
        public:
            LogMessageData(const char* file, unsigned int line, LogId id, LogSeverity severity,
                const char* tag, int error)
                : file_(GetFileBasename(file)),
                line_number_(line),
                id_(id),
                severity_(severity),
                tag_(tag),
                error_(error) {}
            const char* GetFile() const {
                return file_;
            }
            unsigned int GetLineNumber() const {
                return line_number_;
            }
            LogSeverity GetSeverity() const {
                return severity_;
            }
            const char* GetTag() const { return tag_; }
            LogId GetId() const {
                return id_;
            }
            int GetError() const {
                return error_;
            }
            std::ostream& GetBuffer() {
                return buffer_;
            }
            std::string ToString() const {
                return buffer_.str();
            }
        private:
            std::ostringstream buffer_;
            const char* const file_;
            const unsigned int line_number_;
            const LogId id_;
            const LogSeverity severity_;
            const char* const tag_;
            const int error_;
            DISALLOW_COPY_AND_ASSIGN(LogMessageData);
        };
        LogMessage::LogMessage(const char* file, unsigned int line, LogId id, LogSeverity severity,
            const char* tag, int error)
            : data_(new LogMessageData(file, line, id, severity, tag, error)) {}
        LogMessage::LogMessage(const char* file, unsigned int line, LogId id, LogSeverity severity,
            int error)
            : LogMessage(file, line, id, severity, nullptr, error) {}
        LogMessage::~LogMessage() {
            // Check severity again. This is duplicate work wrt/ LOG macros, but not LOG_STREAM.
            if (!WOULD_LOG(data_->GetSeverity())) {
                return;
            }
            // Finish constructing the message.
            if (data_->GetError() != -1) {
                int error = data_->GetError();
                char buffer[80];
                strerror_s(buffer, 80, error);
                data_->GetBuffer() << ": " << buffer;
            }
            std::string msg(data_->ToString());
            {
                // Do the actual logging with the lock held.
                std::lock_guard<std::mutex> lock(LoggingLock());
                if (msg.find('\n') == std::string::npos) {
                    LogLine(data_->GetFile(), data_->GetLineNumber(), data_->GetId(), data_->GetSeverity(),
                        data_->GetTag(), msg.c_str());
                }
                else {
                    msg += '\n';
                    size_t i = 0;
                    while (i < msg.size()) {
                        size_t nl = msg.find('\n', i);
                        msg[nl] = '\0';
                        LogLine(data_->GetFile(), data_->GetLineNumber(), data_->GetId(), data_->GetSeverity(),
                            data_->GetTag(), &msg[i]);
                        // Undo the zero-termination so we can give the complete message to the aborter.
                        msg[nl] = '\n';
                        i = nl + 1;
                    }
                }
            }
            // Abort if necessary.
            if (data_->GetSeverity() == FATAL) {
                Aborter()(msg.c_str());
            }
        }
        std::ostream& LogMessage::stream() {
            return data_->GetBuffer();
        }
        void LogMessage::LogLine(const char* file, unsigned int line, LogId id, LogSeverity severity,
            const char* tag, const char* message) {
            if (tag == nullptr) {
                std::lock_guard<std::recursive_mutex> lock(TagLock());
                if (gDefaultTag == nullptr) {
                    gDefaultTag = new std::string(getprogname());
                }
                Logger()(id, severity, gDefaultTag->c_str(), file, line, message);
            }
            else {
                Logger()(id, severity, tag, file, line, message);
            }
        }
        void LogMessage::LogLine(const char* file, unsigned int line, LogId id, LogSeverity severity,
            const char* message) {
            LogLine(file, line, id, severity, nullptr, message);
        }
        LogSeverity GetMinimumLogSeverity() {
            return gMinimumLogSeverity;
        }
        LogSeverity SetMinimumLogSeverity(LogSeverity new_severity) {
            LogSeverity old_severity = gMinimumLogSeverity;
            gMinimumLogSeverity = new_severity;
            return old_severity;
        }
        ScopedLogSeverity::ScopedLogSeverity(LogSeverity new_severity) {
            old_ = SetMinimumLogSeverity(new_severity);
        }
        ScopedLogSeverity::~ScopedLogSeverity() {
            SetMinimumLogSeverity(old_);
        }
    }  // namespace base
}  // namespace android