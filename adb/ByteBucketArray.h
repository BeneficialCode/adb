#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>


#include "logging.h"

namespace android {

    /**
     * Stores a sparsely populated array. Has a fixed size of 256
     * (number of entries that a byte can represent).
     */
    template <typename T>
    class ByteBucketArray {
    public:
        ByteBucketArray() {
            memset(buckets_, 0, sizeof(buckets_));
        }

        ~ByteBucketArray() {
            deleteBuckets();
        }

        void clear() {
            deleteBuckets();
            memset(buckets_, 0, sizeof(buckets_));
        }

        inline size_t size() const { return kNumBuckets * kBucketSize; }

        inline const T& get(size_t index) const { return (*this)[index]; }

        const T& operator[](size_t index) const {
            if (index >= size()) {
                return default_;
            }

            uint8_t bucket_index = static_cast<uint8_t>(index) >> 4;
            T* bucket = buckets_[bucket_index];
            if (bucket == nullptr) {
                return default_;
            }
            return bucket[0x0f & static_cast<uint8_t>(index)];
        }

        T& editItemAt(size_t index) {
            CHECK(index < size()) << "ByteBucketArray.editItemAt(index=" << index
                << ") with size=" << size();

            uint8_t bucket_index = static_cast<uint8_t>(index) >> 4;
            T*& bucket = buckets_[bucket_index];
            if (bucket == nullptr) {
                bucket = new T[kBucketSize]();
            }
            return bucket[0x0f & static_cast<uint8_t>(index)];
        }

        bool set(size_t index, const T& value) {
            if (index >= size()) {
                return false;
            }

            editItemAt(index) = value;
            return true;
        }

        template <class Func>
        void forEachItem(Func f) {
            for (size_t i = 0; i < kNumBuckets; i++) {
                const auto bucket = buckets_[i];
                if (bucket != nullptr) {
                    for (size_t j = 0; j < kBucketSize; j++) {
                        f((i << 4) | j, bucket[j]);
                    }
                }
            }
        }

        template <class Func>
        void trimBuckets(Func isEmptyFunc) {
            for (size_t i = 0; i < kNumBuckets; i++) {
                const auto bucket = buckets_[i];
                if (bucket != nullptr) {
                    if (std::all_of(bucket, bucket + kBucketSize, isEmptyFunc)) {
                        delete[] bucket;
                        buckets_[i] = nullptr;
                    }
                }
            }
        }

    private:
        enum { kNumBuckets = 16, kBucketSize = 16 };

        void deleteBuckets() {
            for (size_t i = 0; i < kNumBuckets; i++) {
                if (buckets_[i] != nullptr) {
                    delete[] buckets_[i];
                }
            }
        }

        T* buckets_[kNumBuckets];
        static inline const T default_ = {};
    };

}  // namespace android
