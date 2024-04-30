#pragma once

#include <stdint.h>
#include <sys/types.h>
#include <time.h>


#include <condition_variable>
#include "errors.h"
#include <mutex>
#include "RefBase.h"
#include "Timers.h"
#include "ThreadDefs.h"

// ---------------------------------------------------------------------------
namespace android {
    // ---------------------------------------------------------------------------

    // DO NOT USE: please use std::thread

    class Thread : virtual public RefBase
    {
    public:
        // Create a Thread object, but doesn't create or start the associated
        // thread. See the run() method. This object must be used with RefBase/sp,
        // like any other RefBase object, because they are conventionally promoted
        // from bare pointers (Thread::run is particularly problematic here).
        explicit            Thread(bool canCallJava = true);
        virtual             ~Thread();

        // Start the thread in threadLoop() which needs to be implemented.
        // NOLINTNEXTLINE(google-default-arguments)
        virtual status_t    run(const char* name,
            int32_t priority = PRIORITY_DEFAULT,
            size_t stack = 0);

        // Ask this object's thread to exit. This function is asynchronous, when the
        // function returns the thread might still be running. Of course, this
        // function can be called from a different thread.
        virtual void        requestExit();

        // Good place to do one-time initializations
        virtual status_t    readyToRun();

        // Call requestExit() and wait until this object's thread exits.
        // BE VERY CAREFUL of deadlocks. In particular, it would be silly to call
        // this function from this object's thread. Will return WOULD_BLOCK in
        // that case.
        status_t    requestExitAndWait();

        // Wait until this object's thread exits. Returns immediately if not yet running.
        // Do not call from this object's thread; will return WOULD_BLOCK in that case.
        status_t    join();

        // Indicates whether this thread is running or not.
        bool        isRunning() const;

#if defined(__ANDROID__)
    
#endif

    protected:
        // exitPending() returns true if requestExit() has been called.
        bool        exitPending() const;

    private:
        // Derived class must implement threadLoop(). The thread starts its life
        // here. There are two ways of using the Thread object:
        // 1) loop: if threadLoop() returns true, it will be called again if
        //          requestExit() wasn't called.
        // 2) once: if threadLoop() returns false, the thread will exit upon return.
        virtual bool        threadLoop() = 0;

    private:
        Thread& operator=(const Thread&);
        static  int             _threadLoop(void* user);
        const   bool            mCanCallJava;
        // always hold mLock when reading or writing
        thread_id_t     mThread;
        mutable std::mutex           mLock;
        std::condition_variable      mThreadExitedCondition;
        status_t        mStatus;
        // note that all accesses of mExitPending and mRunning need to hold mLock
        volatile bool           mExitPending;
        volatile bool           mRunning;
        sp<Thread>      mHoldSelf;
#if defined(__ANDROID__)
       
#endif
    };

}  // namespace android