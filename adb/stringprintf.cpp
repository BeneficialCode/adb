#include "stringprintf.h"

#include <stdio.h>
#include <string>
#include <iomanip>
#include <sstream>

namespace android {
    namespace base {
        void StringAppendV(std::string* dst, const char* format, va_list ap) {
            // First try with a small fixed size buffer
            char space[1024] = { 0 };
            // It's possible for methods that use a va_list to invalidate
            // the data in it upon use.  The fix is to make a copy
            // of the structure before using it and use that copy instead.
            va_list backup_ap;
            va_copy(backup_ap, ap);
            int result = vsnprintf(space, sizeof(space), format, backup_ap);
            va_end(backup_ap);
            if (result < static_cast<int>(sizeof(space))) {
                if (result >= 0) {
                    // Normal case -- everything fit.
                    dst->append(space, result);
                    return;
                }
                if (result < 0) {
                    // Just an error.
                    return;
                }
            }
            // Increase the buffer size to the size requested by vsnprintf,
            // plus one for the closing \0.
            int length = result + 1;
            char* buf = new char[length];
            // Restore the va_list before we use it again
            va_copy(backup_ap, ap);
            result = vsnprintf(buf, length, format, backup_ap);
            va_end(backup_ap);
            if (result >= 0 && result < length) {
                // It fit
                dst->append(buf, result);
            }
            delete[] buf;
        }
        std::string StringPrintf(const char* fmt, ...) {
            va_list ap;
            va_start(ap, fmt);
            std::string result;
            StringAppendV(&result, fmt, ap);
            va_end(ap);
            return result;
        }
        void StringAppendF(std::string* dst, const char* format, ...) {
            va_list ap;
            va_start(ap, format);
            StringAppendV(dst, format, ap);
            va_end(ap);
        }
    }  // namespace base
}  // namespace android

namespace openscreen {

    std::string StringPrintf(const char* format, ...) {
        va_list vlist;
        va_start(vlist, format);
        const int length = std::vsnprintf(nullptr, 0, format, vlist);
        va_end(vlist);

        std::string result(length, '\0');
        // Note: There's no need to add one for the extra terminating NUL char since
        // the standard, since C++11, requires that "data() + size() points to [the
        // NUL terminator]". Thus, std::vsnprintf() will write the NUL to a valid
        // memory location.
        va_start(vlist, format);
        std::vsnprintf(&result[0], length + 1, format, vlist);
        va_end(vlist);

        return result;
    }

    std::string HexEncode(const uint8_t* bytes, std::size_t len) {
        std::ostringstream hex_dump;
        hex_dump << std::setfill('0') << std::hex;
        for (std::size_t i = 0; i < len; i++) {
            hex_dump << std::setw(2) << static_cast<int>(bytes[i]);
        }
        return hex_dump.str();
    }

}  // namespace openscreen