#pragma once

#include <atomic>

#include <sys/types.h>

namespace android {

    class ReferenceRenamer;

    void LightRefBase_reportIncStrongRequireStrongFailed(const void* thiz);

    template <class T>
    class LightRefBase
    {
    public:
        inline LightRefBase() : mCount(0) { }
        inline void incStrong(const void* id) const {
            mCount.fetch_add(1, std::memory_order_relaxed);
        }
        inline void incStrongRequireStrong(const void* id) const {
            if (0 == mCount.fetch_add(1, std::memory_order_relaxed)) {
                LightRefBase_reportIncStrongRequireStrongFailed(this);
            }
        }
        inline void decStrong(const void* id) const {
            if (mCount.fetch_sub(1, std::memory_order_release) == 1) {
                std::atomic_thread_fence(std::memory_order_acquire);
                delete static_cast<const T*>(this);
            }
        }
        //! DEBUGGING ONLY: Get current strong ref count.
        inline int32_t getStrongCount() const {
            return mCount.load(std::memory_order_relaxed);
        }

    protected:
        inline ~LightRefBase() { }

    private:
        friend class ReferenceMover;
        inline static void renameRefs(size_t /*n*/, const ReferenceRenamer& /*renamer*/) { }
        inline static void renameRefId(T* /*ref*/, const void* /*old_id*/, const void* /*new_id*/) { }

    private:
        mutable std::atomic<int32_t> mCount;
    };

    // This is a wrapper around LightRefBase that simply enforces a virtual
    // destructor to eliminate the template requirement of LightRefBase
    class VirtualLightRefBase : public LightRefBase<VirtualLightRefBase> {
    public:
        virtual ~VirtualLightRefBase() = default;
    };

}  // namespace android