#pragma once

#include "ResourceTypes.h"
#include "ByteOrder.h"

namespace android {

    struct TypeVariant {
        explicit TypeVariant(const ResTable_type* data);

        class iterator {
        public:
            iterator& operator=(const iterator& rhs) {
                mTypeVariant = rhs.mTypeVariant;
                mIndex = rhs.mIndex;
                return *this;
            }

            bool operator==(const iterator& rhs) const {
                return mTypeVariant == rhs.mTypeVariant && mIndex == rhs.mIndex;
            }

            bool operator!=(const iterator& rhs) const {
                return mTypeVariant != rhs.mTypeVariant || mIndex != rhs.mIndex;
            }

            iterator operator++(int) {
                uint32_t prevIndex = mIndex;
                operator++();
                return iterator(mTypeVariant, prevIndex);
            }

            const ResTable_entry* operator->() const {
                return operator*();
            }

            uint32_t index() const {
                return mIndex;
            }

            iterator& operator++();
            const ResTable_entry* operator*() const;

        private:
            friend struct TypeVariant;
            iterator(const TypeVariant* tv, uint32_t index)
                : mTypeVariant(tv), mIndex(index) {}
            const TypeVariant* mTypeVariant;
            uint32_t mIndex;
        };

        iterator beginEntries() const {
            return iterator(this, 0);
        }

        iterator endEntries() const {
            return iterator(this, mLength);
        }

        const ResTable_type* data;

    private:
        size_t mLength;
    };

} // namespace android
