#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <ostream>
#include <string>

namespace android {
	namespace base {
		// Returns a string corresponding to printf-like formatting of the arguments.
		std::string StringPrintf(const char* fmt, ...);
		// Appends a printf-like formatting of the arguments to 'dst'.
		void StringAppendF(std::string* dst, const char* fmt, ...);
		// Appends a printf-like formatting of the arguments to 'dst'.
		void StringAppendV(std::string* dst, const char* format, va_list ap);
	}  // namespace base
}  // namespace android

namespace openscreen {
// Returns a std::string containing the results of a std::sprintf() call. This
// is an efficient, zero-copy wrapper.
    [[nodiscard]] std::string StringPrintf(const char* format, ...);

    template <typename It>
    void PrettyPrintAsciiHex(std::ostream& os, It first, It last) {
        auto it = first;
        while (it != last) {
            uint8_t c = *it++;
            if (c >= ' ' && c <= '~') {
                os.put(c);
            }
            else {
                // Output a hex escape sequence for non-printable values.
                os.put('\\');
                os.put('x');
                char digit = (c >> 4) & 0xf;
                if (digit >= 0 && digit <= 9) {
                    os.put(digit + '0');
                }
                else {
                    os.put(digit - 10 + 'a');
                }
                digit = c & 0xf;
                if (digit >= 0 && digit <= 9) {
                    os.put(digit + '0');
                }
                else {
                    os.put(digit - 10 + 'a');
                }
            }
        }
    }

    // Returns a hex string representation of the given |bytes|.
    std::string HexEncode(const uint8_t* bytes, std::size_t len);

}  // namespace openscreen