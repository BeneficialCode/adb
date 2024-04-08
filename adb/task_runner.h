#pragma once

#include <future>
#include <utility>

#include "time.h"

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <optional>
#include <thread>

#include "thread_annotations.h"

namespace openscreen {

    // A thread-safe API surface that allows for posting tasks. The underlying
    // implementation may be single or multi-threaded, and all complication should
    // be handled by the implementation class. The implementation must guarantee:
    // (1) Tasks shall not overlap in time/CPU.
    // (2) Tasks shall run sequentially, e.g. posting task A then B implies
    //     that A shall run before B.
    // (3) If task A is posted before task B, then any mutation in A happens-before
    //     B runs (even if A and B run on different threads).
    class TaskRunner {
    public:
        // Seem to get an error using clang when compiling with -fno-exceptions:
        //   error: implicit instantiation of undefined template 'std::__1::packaged_task<void () noexcept>'
#if __has_feature(cxx_exceptions)
        using Task = std::packaged_task<void() noexcept>;
#else
        using Task = std::packaged_task<void()>;
#endif

        virtual ~TaskRunner() = default;

        // Takes any callable target (function, lambda-expression, std::bind result,
        // etc.) that should be run at the first convenient time.
        template <typename Functor>
        inline void PostTask(Functor f) {
            PostPackagedTask(Task(std::move(f)));
        }

        // Takes any callable target (function, lambda-expression, std::bind result,
        // etc.) that should be run no sooner than |delay| time from now. Note that
        // the Task might run after an additional delay, especially under heavier
        // system load. There is no deadline concept.
        template <typename Functor>
        inline void PostTaskWithDelay(Functor f, Clock::duration delay) {
            PostPackagedTaskWithDelay(Task(std::move(f)), delay);
        }

        // Implementations should provide the behavior explained in the comments above
        // for PostTask[WithDelay](). Client code may also call these directly when
        // passing an existing Task object.
        virtual void PostPackagedTask(Task task) = 0;
        virtual void PostPackagedTaskWithDelay(Task task, Clock::duration delay) = 0;

        // Return true if the calling thread is the thread that task runner is using
        // to run tasks, false otherwise.
        virtual bool IsRunningOnTaskRunner() = 0;
    };

}  // namespace openscreen

namespace mdns {

    // Openscreen API surface that allows for posting tasks. The underlying
    // implementation may be single or multi-threaded, and all complication should
    // be handled by the implementation class. The implementation must guarantee:
    // (1) Tasks shall not overlap in time/CPU.
    // (2) Tasks shall run sequentially, e.g. posting task A then B implies
    //     that A shall run before B.
    // (3) If task A is posted before task B, then any mutation in A happens-before
    //     B runs (even if A and B run on different threads).
    //
    // Adb implementation: The PostPackagedTask* APIs are thread-safe.
    // Another thread will handle dequeuing each item and calling it on the fdevent thread.
    // Thus, the task runner thread is the fdevent thread, and IsRunningOnTaskRunner() shall always
    // return true if calling from within the running Task.
    class AdbOspTaskRunner final : public openscreen::TaskRunner {
    public:
        using Task = openscreen::TaskRunner::Task;

        // Must be called on the fdevent thread.
        explicit AdbOspTaskRunner();
        ~AdbOspTaskRunner() final;
        void PostPackagedTask(Task task) final;
        void PostPackagedTaskWithDelay(Task task, openscreen::Clock::duration delay) final;
        bool IsRunningOnTaskRunner() final;

        // The task executor thread.
        void TaskExecutorWorker();

    private:
        uint64_t thread_id_;
        std::mutex mutex_;
        _Guarded_by_(mutex_) std::multimap<std::chrono::time_point<std::chrono::steady_clock>, Task> tasks_;
        std::atomic<bool> terminate_loop_ = false;
        std::condition_variable cv_;
        std::thread task_handler_;
    };

}  // namespace mdns