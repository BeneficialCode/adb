#include "mdns_utils.h"
#include "strings.h"
#include <array>
#include <string>

namespace mdns {

    // <Instance>.<Service>.<Domain>
    std::optional<MdnsInstance> mdns_parse_instance_name(std::string_view name) {
        CHECK(!name.empty());

        // Return the whole name if it doesn't fall under <Instance>.<Service>.<Domain> or
        // <Instance>.<Service>
        bool has_local_suffix = false;
        // Strip the local suffix, if any
        {
            std::string local_suffix = ".local";
            local_suffix += android::base::EndsWith(name, ".") ? "." : "";

            if (android::base::ConsumeSuffix(&name, local_suffix)) {
                if (name.empty()) {
                    return std::nullopt;
                }
                has_local_suffix = true;
            }
        }

        std::string transport;
        // Strip the transport suffix, if any
        {
            std::string add_dot = (!has_local_suffix && android::base::EndsWith(name, ".")) ? "." : "";
            std::array<std::string, 2> transport_suffixes{ "._tcp", "._udp" };

            for (const auto& t : transport_suffixes) {
                if (android::base::ConsumeSuffix(&name, t + add_dot)) {
                    if (name.empty()) {
                        return std::nullopt;
                    }
                    transport = t.substr(1);
                    break;
                }
            }

            if (has_local_suffix && transport.empty()) {
                return std::nullopt;
            }
        }

        if (!has_local_suffix && transport.empty()) {
            return std::make_optional<MdnsInstance>(name, "", "");
        }

        // Split the service name from the instance name
        auto pos = name.rfind(".");
        if (pos == 0 || pos == std::string::npos || pos == name.size() - 1) {
            return std::nullopt;
        }

        return std::make_optional<MdnsInstance>(name.substr(0, pos), name.substr(pos + 1), transport);
    }

}  // namespace mdns