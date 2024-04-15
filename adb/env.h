#pragma once

#include <optional>
#include <string>

namespace adb {
	namespace sysdeps {

		// Attempts to retrieve the environment variable value for |var|. Returns std::nullopt
		// if unset.
		std::optional<std::string> GetEnvironmentVariableUTF8(std::string_view var);

		// Gets the host name of the system. Returns empty string on failure.
		std::string GetHostNameUTF8();
		// Gets the current login user. Returns empty string on failure.
		std::string GetLoginNameUTF8();

	}  // namespace sysdeps
}  // namespace adb