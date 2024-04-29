#pragma once

#include <stdint.h>
#include <sys/types.h>

#if !defined(_WIN32)

#endif

#include "ThreadDefs.h"

// ---------------------------------------------------------------------------
// C API

#ifdef __cplusplus
extern "C" {
#endif

    // Create and run a new thread.
    extern int androidCreateThread(android_thread_func_t, void*);

    // Create thread with lots of parameters
    extern int androidCreateThreadEtc(android_thread_func_t entryFunction,
        void* userData,
        const char* threadName,
        int32_t threadPriority,
        size_t threadStackSize,
        android_thread_id_t* threadId);

    // Get some sort of unique identifier for the current thread.
    extern android_thread_id_t androidGetThreadId();

    // Low-level thread creation -- never creates threads that can
    // interact with the Java VM.
    extern int androidCreateRawThreadEtc(android_thread_func_t entryFunction,
        void* userData,
        const char* threadName,
        int32_t threadPriority,
        size_t threadStackSize,
        android_thread_id_t* threadId);

    // set the same of the running thread
    extern void androidSetThreadName(const char* name);

    // Used by the Java Runtime to control how threads are created, so that
    // they can be proper and lovely Java threads.
    typedef int (*android_create_thread_fn)(android_thread_func_t entryFunction,
        void* userData,
        const char* threadName,
        int32_t threadPriority,
        size_t threadStackSize,
        android_thread_id_t* threadId);

    extern void androidSetCreateThreadFunc(android_create_thread_fn func);

    // ------------------------------------------------------------------
    // Extra functions working with raw pids.

#if defined(__ANDROID__)

#endif

#ifdef __cplusplus
} // extern "C"
#endif

// ----------------------------------------------------------------------------
// C++ API
#ifdef __cplusplus
namespace android {
    // ----------------------------------------------------------------------------

    // Create and run a new thread.
    inline bool createThread(thread_func_t f, void* a) {
        return androidCreateThread(f, a) ? true : false;
    }

    // Create thread with lots of parameters
    inline bool createThreadEtc(thread_func_t entryFunction,
        void* userData,
        const char* threadName = "android:unnamed_thread",
        int32_t threadPriority = PRIORITY_DEFAULT,
        size_t threadStackSize = 0,
        thread_id_t* threadId = nullptr)
    {
        return androidCreateThreadEtc(entryFunction, userData, threadName,
            threadPriority, threadStackSize, threadId) ? true : false;
    }

    // Get some sort of unique identifier for the current thread.
    inline thread_id_t getThreadId() {
        return androidGetThreadId();
    }

    // ----------------------------------------------------------------------------
}  // namespace android
#endif  // __cplusplus
// ----------------------------------------------------------------------------