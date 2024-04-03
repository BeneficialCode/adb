#pragma once

#include <string>

namespace android {
	namespace base {
		bool WideToUTF8(const wchar_t* utf16, const size_t size, std::string* utf8);
		// Convert a NULL-terminated string of UTF-16 characters to UTF-8. Returns
		// whether the conversion was done successfully.
		bool WideToUTF8(const wchar_t* utf16, std::string* utf8);
		// Convert a UTF-16 std::wstring (including any embedded NULL characters) to
		// UTF-8. Returns whether the conversion was done successfully.
		bool WideToUTF8(const std::wstring& utf16, std::string* utf8);
		// Convert size number of UTF-8 char's to UTF-16. Returns whether the conversion
		// was done successfully.
		bool UTF8ToWide(const char* utf8, const size_t size, std::wstring* utf16);
		// Convert a NULL-terminated string of UTF-8 characters to UTF-16. Returns
		// whether the conversion was done successfully.
		bool UTF8ToWide(const char* utf8, std::wstring* utf16);
		// Convert a UTF-8 std::string (including any embedded NULL characters) to
		// UTF-16. Returns whether the conversion was done successfully.
		bool UTF8ToWide(const std::string& utf8, std::wstring* utf16);

		namespace utf8 {
			int open(const char* name, int flags, ...);
			int unlink(const char* name);
		}  // namespace utf8
	}  // namespace base
}  // namespace android