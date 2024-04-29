#pragma once

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <string>

namespace android {
	namespace base {
		// Returns a string describing the given system error code. |error_code| must
		// be errno on Unix or GetLastError()/WSAGetLastError() on Windows. Passing
		// errno on Windows has undefined behavior.
		std::string SystemErrorCodeToString(int error_code);
	}  // namespace base

    /**
 * The type used to return success/failure from frameworks APIs.
 * See the anonymous enum below for valid values.
 */
    typedef int32_t status_t;

    /*
     * Error codes.
     * All error codes are negative values.
     */

    enum {
        OK = 0,    // Preferred constant for checking success.
#ifndef NO_ERROR
        // Win32 #defines NO_ERROR as well.  It has the same value, so there's no
        // real conflict, though it's a bit awkward.
        NO_ERROR = OK,   // Deprecated synonym for `OK`. Prefer `OK` because it doesn't conflict with Windows.
#endif

        UNKNOWN_ERROR = (-2147483647 - 1), // INT32_MIN value

        NO_MEMORY = -ENOMEM,
        INVALID_OPERATION = -ENOSYS,
        BAD_VALUE = -EINVAL,
        BAD_TYPE = (UNKNOWN_ERROR + 1),
        NAME_NOT_FOUND = -ENOENT,
        PERMISSION_DENIED = -EPERM,
        NO_INIT = -ENODEV,
        ALREADY_EXISTS = -EEXIST,
        DEAD_OBJECT = -EPIPE,
        FAILED_TRANSACTION = (UNKNOWN_ERROR + 2),
#if !defined(_WIN32)

#else    
        BAD_INDEX = -E2BIG,
        NOT_ENOUGH_DATA = (UNKNOWN_ERROR + 3),
        WOULD_BLOCK = (UNKNOWN_ERROR + 4),
        TIMED_OUT = (UNKNOWN_ERROR + 5),
        UNKNOWN_TRANSACTION = (UNKNOWN_ERROR + 6),
#endif    
        FDS_NOT_ALLOWED = (UNKNOWN_ERROR + 7),
        UNEXPECTED_NULL = (UNKNOWN_ERROR + 8),
    };

    // Human readable name of error
    std::string statusToString(status_t status);
}  // namespace android