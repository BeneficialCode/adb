#pragma once

#include <stddef.h>
#include <stdint.h>

#include <limits>

// Check if |length| bytes at |entry_name| constitute a valid entry name.
// Entry names must be valid UTF-8 and must not contain '0'. They also must
// fit into the central directory record.
inline bool IsValidEntryName(const uint8_t* entry_name, const size_t length) {
    if (length > std::numeric_limits<uint16_t>::max()) {
        return false;
    }
    for (size_t i = 0; i < length; ++i) {
        const uint8_t byte = entry_name[i];
        if (byte == 0) {
            return false;
        }
        else if ((byte & 0x80) == 0) {
            // Single byte sequence.
            continue;
        }
        else if ((byte & 0xc0) == 0x80 || (byte & 0xfe) == 0xfe) {
            // Invalid sequence.
            return false;
        }
        else {
            // 2-5 byte sequences.
            for (uint8_t first = static_cast<uint8_t>((byte & 0x7f) << 1); first & 0x80;
                first = static_cast<uint8_t>((first & 0x7f) << 1)) {
                ++i;

                // Missing continuation byte..
                if (i == length) {
                    return false;
                }

                // Invalid continuation byte.
                const uint8_t continuation_byte = entry_name[i];
                if ((continuation_byte & 0xc0) != 0x80) {
                    return false;
                }
            }
        }
    }

    return true;
}