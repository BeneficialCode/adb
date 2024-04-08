#include "mdns_service_info.h"

#include "adb_mdns.h"

using namespace openscreen;

namespace mdns {

    ErrorOr<ServiceInfo> DnsSdInstanceEndpointToServiceInfo(
        const discovery::DnsSdInstanceEndpoint& endpoint) {
        ServiceInfo service_info;
        // Check if |endpoint| is a known adb service name
        for (int i = 0; i < kNumADBDNSServices; ++i) {
            if (endpoint.service_id() == kADBDNSServices[i]) {
                service_info.service_name = endpoint.service_id();
                service_info.instance_name = endpoint.instance_id();
                break;
            }
            if (i == kNumADBDNSServices - 1) {
                LOG(ERROR) << "Got unknown service name [" << endpoint.service_id() << "]";
                return Error::Code::kParameterInvalid;
            }
        }

        service_info.port = endpoint.port();
        for (const IPAddress& address : endpoint.addresses()) {
            if (!service_info.v4_address && address.IsV4()) {
                service_info.v4_address = address;
            }
            else if (!service_info.v6_address && address.IsV6()) {
                service_info.v6_address = address;
            }
        }
        CHECK(service_info.v4_address || service_info.v6_address);
        return service_info;
    }

}  // namespace mdns