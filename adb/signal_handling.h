#pragma once

#include <sys/types.h>

#if !defined(__BIONIC__) || INCFS_SUPPORT_DISABLED

// IncFS signal handling isn't needed anywhere but on Android as of now;
// or if we're told it's not desired.
#define SCOPED_SIGBUS_HANDLER(code)
#define SCOPED_SIGBUS_HANDLER_CONDITIONAL(condition, code)

#else

#ifndef LOG_TAG
#define LOG_TAG "incfs:hardening"
#endif

#include <setjmp.h>
#include <signal.h>
#include <string.h>

namespace incfs {

    struct JmpBufState final {
        jmp_buf buf;
        bool armed = false;

        JmpBufState() = default;
        JmpBufState(const JmpBufState& other) {
            if (other.armed) {
                memcpy(&buf, &other.buf, sizeof(buf));
                armed = true;
            }
        }

        JmpBufState& operator=(const JmpBufState& other) {
            if (&other != this) {
                if (other.armed) {
                    memcpy(&buf, &other.buf, sizeof(buf));
                    armed = true;
                }
                else {
                    armed = false;
                }
            }
            return *this;
        }
    };

    class ScopedJmpBuf final {
    public:
        ScopedJmpBuf(const JmpBufState& prev) : mPrev(prev) {}
        ~ScopedJmpBuf();

        ScopedJmpBuf(const ScopedJmpBuf&) = delete;

    private:
        const JmpBufState& mPrev;
    };

#define SCOPED_SIGBUS_HANDLER_CONDITIONAL(condition, code)                     \
  (void)incfs::SignalHandler::instance();                                      \
  auto& tlsBuf_macro = incfs::SignalHandler::mJmpBuf;                          \
  incfs::JmpBufState oldBuf_macro = tlsBuf_macro;                              \
  const bool condition_macro_val = (condition);                                \
  if (condition_macro_val && setjmp(tlsBuf_macro.buf) != 0) {                  \
    tlsBuf_macro = oldBuf_macro;                                               \
    do {                                                                       \
      code;                                                                    \
    } while (0);                                                               \
  }                                                                            \
  tlsBuf_macro.armed |= (condition_macro_val);                                 \
  incfs::ScopedJmpBuf oldBufRestore_macro(oldBuf_macro)

#define SCOPED_SIGBUS_HANDLER(code) \
  SCOPED_SIGBUS_HANDLER_CONDITIONAL(true, code)

    class SignalHandler final {
    public:
        static SignalHandler& instance();

    private:
        SignalHandler();
        inline static struct sigaction mOldSigaction = {};

        static void handler(int sig, siginfo_t* info, void* ucontext);

    public:
        inline static thread_local JmpBufState mJmpBuf = {};
    };

}  // namespace incfs

#endif