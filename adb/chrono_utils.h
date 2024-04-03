#pragma once

#include <chrono>
#include <sstream>
#if __cplusplus > 201103L && !defined(__WIN32)  // C++14
using namespace std::chrono_literals;
#endif
namespace android {
    namespace base {
        // A std::chrono clock based on CLOCK_BOOTTIME.
        class boot_clock {
        public:
            typedef std::chrono::nanoseconds duration;
            typedef std::chrono::time_point<boot_clock, duration> time_point;
            static time_point now();
        };
        class Timer {
        public:
            Timer() : start_(boot_clock::now()) {}
            std::chrono::milliseconds duration() const {
                return std::chrono::duration_cast<std::chrono::milliseconds>(boot_clock::now() - start_);
            }
        private:
            boot_clock::time_point start_;
        };
        std::ostream& operator<<(std::ostream& os, const Timer& t);
    }  // namespace base
}  // namespace android