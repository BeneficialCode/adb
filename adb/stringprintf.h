#pragma once

#include <stdarg.h>
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