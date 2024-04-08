#pragma once

#include <string>

#include "dns_sd_instance_endpoint.h"
#include "ip_address.h"

#include "mdns_utils.h"

namespace mdns {

    struct ServiceInfo {
        std::string instance_name;
        std::string service_name;
        openscreen::IPAddress v4_address;
        openscreen::IPAddress v6_address;
        uint16_t port;
    };  // ServiceInfo

    openscreen::ErrorOr<ServiceInfo> DnsSdInstanceEndpointToServiceInfo(
        const openscreen::discovery::DnsSdInstanceEndpoint& endpoint);

}  // namespace mdns