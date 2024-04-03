#pragma once

#include <string>
namespace android {
    namespace base {
        // Parses |address| into |host| and |port|.
        //
        // If |address| doesn't contain a port number, the default value is taken from
        // |port|. If |canonical_address| is non-null it will be set to "host:port" or
        // "[host]:port" as appropriate.
        //
        // On failure, returns false and fills |error|.
        bool ParseNetAddress(const std::string& address, std::string* host, int* port,
            std::string* canonical_address, std::string* error);
    }  // namespace base
}  // namespace android