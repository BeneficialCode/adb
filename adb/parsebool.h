#pragma once

#include <string_view>

namespace android {
	namespace base {

		// Parse the given string as yes or no inactivation of some sort. Return one of the
		// ParseBoolResult enumeration values.
		//
		// The following values parse as true:
		//
		//   1
		//   on
		//   true
		//   y
		//   yes
		//
		//
		// The following values parse as false:
		//
		//   0
		//   false
		//   n
		//   no
		//   off
		//
		// Anything else is a parse error.
		//
		// The purpose of this function is to have a single canonical parser for yes-or-no indications
		// throughout the system.

		enum class ParseBoolResult {
			kError,
			kFalse,
			kTrue,
		};

		ParseBoolResult ParseBool(std::string_view s);

	}  // namespace base
}  // namespace android