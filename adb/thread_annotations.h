#pragma once

#include <mutex>

namespace android {
    namespace base {

        // A class to help thread safety analysis deal with std::unique_lock and condition_variable.
        //
        // Clang's thread safety analysis currently doesn't perform alias analysis, so movable types
        // like std::unique_lock can't be marked with thread safety annotations. This helper allows
        // for manual assertion of lock state in a scope.
        //
        // For example:
        //
        //   std::mutex mutex;
        //   std::condition_variable cv;
        //   std::vector<int> vec GUARDED_BY(mutex);
        //
        //   int pop() {
        //     std::unique_lock lock(mutex);
        //     ScopedLockAssertion lock_assertion(mutex);
        //     cv.wait(lock, []() {
        //       ScopedLockAssertion lock_assertion(mutex);
        //       return !vec.empty();
        //     });
        //
        //     int result = vec.back();
        //     vec.pop_back();
        //     return result;
        //   }
        class ScopedLockAssertion {
        public:
            ScopedLockAssertion(std::mutex& mutex)  {}
            ~ScopedLockAssertion() {}
        };

    }  // namespace base
}  // namespace android