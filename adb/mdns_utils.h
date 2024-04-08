#pragma once
#include <optional>
#include <string_view>

#include "adb_wifi.h"

namespace mdns {

    struct MdnsInstance {
        std::string instance_name;   // "my name"
        std::string service_name;    // "_adb-tls-connect"
        std::string transport_type;  // either "_tcp" or "_udp"

        MdnsInstance(std::string_view inst, std::string_view serv, std::string_view trans)
            : instance_name(inst), service_name(serv), transport_type(trans) {}
    };

    // This parser is based on https://tools.ietf.org/html/rfc6763#section-4.1 for
    // structured service instance names, where the whole name is in the format
    // <Instance>.<Service>.<Domain>.
    //
    // In our case, we ignore <Domain> portion of the name, which
    // we always assume to be ".local", or link-local mDNS.
    //
    // The string can be in one of the following forms:
    //   - <Instance>.<Service>.<Domain>.?
    //     - e.g. "instance._service._tcp.local" (or "...local.")
    //   - <Instance>.<Service>.? (must contain either "_tcp" or "_udp" at the end)
    //     - e.g. "instance._service._tcp" (or "..._tcp.)
    //   - <Instance> (can contain dots '.')
    //     - e.g. "myname", "name.", "my.name."
    //
    // Returns an MdnsInstance with the appropriate fields filled in (instance name is never empty),
    // otherwise returns std::nullopt.
    std::optional<MdnsInstance> mdns_parse_instance_name(std::string_view name);

}  // namespace mdns