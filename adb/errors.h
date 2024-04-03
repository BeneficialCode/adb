#pragma once

#include <string>

namespace android {
	namespace base {
		// Returns a string describing the given system error code. |error_code| must
		// be errno on Unix or GetLastError()/WSAGetLastError() on Windows. Passing
		// errno on Windows has undefined behavior.
		std::string SystemErrorCodeToString(int error_code);
	}  // namespace base
}  // namespace android