#include "alarm.h"

namespace openscreen {

    class Alarm::CancelableFunctor {
    public:
        explicit CancelableFunctor(Alarm* alarm) : alarm_(alarm) {
            alarm_->queued_fire_ = this;
        }

        ~CancelableFunctor() { Cancel(); }

        CancelableFunctor(CancelableFunctor&& other) : alarm_(other.alarm_) {
            other.alarm_ = nullptr;
            if (alarm_) {
                alarm_->queued_fire_ = this;
            }
        }
        // 复制构造函数
        CancelableFunctor(const CancelableFunctor& other):alarm_(other.alarm_) {
        }

        CancelableFunctor& operator=(CancelableFunctor&& other) {
            Cancel();
            alarm_ = other.alarm_;
            other.alarm_ = nullptr;
            if (alarm_) {
                alarm_->queued_fire_ = this;
            }
            return *this;
        }

        void operator()() noexcept {
            if (alarm_) {
                alarm_->queued_fire_ = nullptr;
                alarm_->TryInvoke();
                alarm_ = nullptr;
            }
        }

        void Cancel() {
            if (alarm_) {
                alarm_->queued_fire_ = nullptr;
                alarm_ = nullptr;
            }
        }

    private:
        Alarm* alarm_;
    };

    Alarm::Alarm(ClockNowFunctionPtr now_function, TaskRunner* task_runner)
        : now_function_(now_function), task_runner_(task_runner) {
    }

    Alarm::~Alarm() {
        if (queued_fire_) {
            queued_fire_->Cancel();
        }
    }

    void Alarm::Cancel() {
        scheduled_task_ = TaskRunner::Task();
    }

    void Alarm::ScheduleWithTask(TaskRunner::Task task,
        Clock::time_point desired_alarm_time) {

        scheduled_task_ = std::move(task);

        const Clock::time_point now = now_function_();
        alarm_time_ = std::max(now, desired_alarm_time);

        // Ensure that a later firing will occur, and not too late.
        if (queued_fire_) {
            if (next_fire_time_ <= alarm_time_) {
                return;
            }
            queued_fire_->Cancel();
        }
        InvokeLater(now, alarm_time_);
    }

    void Alarm::InvokeLater(Clock::time_point now, Clock::time_point fire_time) {
        next_fire_time_ = fire_time;
        // Note: Instantiating the CancelableFunctor below sets |this->queued_fire_|.
        task_runner_->PostTaskWithDelay(CancelableFunctor(this), fire_time - now);
    }

    void Alarm::TryInvoke() {
        if (!scheduled_task_.valid()) {
            return;  // This Alarm was canceled in the meantime.
        }

        // If this is an early firing, re-schedule for later. This happens if
        // Schedule() was called again before this firing had occurred.
        const Clock::time_point now = now_function_();
        if (now < alarm_time_) {
            InvokeLater(now, alarm_time_);
            return;
        }

        // Move the client Task to the stack before executing, just in case the task
        // itself: a) calls any Alarm methods re-entrantly, or b) causes the
        // destruction of this Alarm instance.
        // WARNING: |this| is not valid after here!
        TaskRunner::Task task = std::move(scheduled_task_);
        task();
    }

    // static
    constexpr Clock::time_point Alarm::kImmediately;

}  // namespace openscreen