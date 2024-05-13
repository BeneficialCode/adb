#include "TypeWrappers.h"

#include <algorithm>

namespace android {

    TypeVariant::TypeVariant(const ResTable_type* data) : data(data), mLength(dtohl(data->entryCount)) {
        if (data->flags & ResTable_type::FLAG_SPARSE) {
            const uint32_t entryCount = dtohl(data->entryCount);
            const uintptr_t containerEnd = reinterpret_cast<uintptr_t>(data) + dtohl(data->header.size);
            const uint32_t* const entryIndices = reinterpret_cast<const uint32_t*>(
                reinterpret_cast<uintptr_t>(data) + dtohs(data->header.headerSize));
            if (reinterpret_cast<uintptr_t>(entryIndices) + (sizeof(uint32_t) * entryCount)
                > containerEnd) {
                mLength = 0;
            }
            else {
                mLength = ResTable_sparseTypeEntry{ entryIndices[entryCount - 1] }.idx + 1;
            }
        }
    }

    TypeVariant::iterator& TypeVariant::iterator::operator++() {
        mIndex++;
        if (mIndex > mTypeVariant->mLength) {
            mIndex = mTypeVariant->mLength;
        }
        return *this;
    }

    static bool keyCompare(uint32_t entry, uint16_t index) {
        return dtohs(ResTable_sparseTypeEntry{ entry }.idx) < index;
    }

    const ResTable_entry* TypeVariant::iterator::operator*() const {
        const ResTable_type* type = mTypeVariant->data;
        if (mIndex >= mTypeVariant->mLength) {
            return NULL;
        }

        const uint32_t entryCount = dtohl(mTypeVariant->data->entryCount);
        const uintptr_t containerEnd = reinterpret_cast<uintptr_t>(type)
            + dtohl(type->header.size);
        const uint32_t* const entryIndices = reinterpret_cast<const uint32_t*>(
            reinterpret_cast<uintptr_t>(type) + dtohs(type->header.headerSize));
        const size_t indexSize = type->flags & ResTable_type::FLAG_OFFSET16 ?
            sizeof(uint16_t) : sizeof(uint32_t);
        if (reinterpret_cast<uintptr_t>(entryIndices) + (indexSize * entryCount) > containerEnd) {
            return NULL;
        }

        uint32_t entryOffset;
        if (type->flags & ResTable_type::FLAG_SPARSE) {
            auto iter = std::lower_bound(entryIndices, entryIndices + entryCount, mIndex, keyCompare);
            if (iter == entryIndices + entryCount
                || dtohs(ResTable_sparseTypeEntry{ *iter }.idx) != mIndex) {
                return NULL;
            }

            entryOffset = static_cast<uint32_t>(dtohs(ResTable_sparseTypeEntry{ *iter }.offset)) * 4u;
        }
        else if (type->flags & ResTable_type::FLAG_OFFSET16) {
            auto entryIndices16 = reinterpret_cast<const uint16_t*>(entryIndices);
            entryOffset = offset_from16(entryIndices16[mIndex]);
        }
        else {
            entryOffset = dtohl(entryIndices[mIndex]);
        }

        if (entryOffset == ResTable_type::NO_ENTRY) {
            return NULL;
        }

        if ((entryOffset & 0x3) != 0) {
            return NULL;
        }

        const ResTable_entry* entry = reinterpret_cast<const ResTable_entry*>(
            reinterpret_cast<uintptr_t>(type) + dtohl(type->entriesStart) + entryOffset);
        if (reinterpret_cast<uintptr_t>(entry) > containerEnd - sizeof(*entry)) {
            return NULL;
        }
        else if (reinterpret_cast<uintptr_t>(entry) + entry->size() > containerEnd) {
            return NULL;
        }
        else if (entry->size() < sizeof(*entry)) {
            
            return NULL;
        }
        return entry;
    }

} // namespace android