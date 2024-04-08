#include "mdns_service_watcher.h"

#include "mdns_service_info.h"

using namespace openscreen;

namespace mdns {

    ServiceReceiver::ServiceReceiver(
        discovery::DnsSdService* service, std::string_view service_name,
        openscreen::discovery::DnsSdServiceWatcher<ServiceInfo>::ServicesUpdatedCallback cb)
        : discovery::DnsSdServiceWatcher<ServiceInfo>(
            service, service_name.data(), DnsSdInstanceEndpointToServiceInfo, std::move(cb)) {
        LOG(VERBOSE) << "Initializing ServiceReceiver service=" << service_name;
    }
}  // namespace mdns