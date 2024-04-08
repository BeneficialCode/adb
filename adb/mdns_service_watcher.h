#pragma once

#include <string_view>

#include "mdns_service_info.h"
#include "dns_sd_service_watcher.h"

namespace mdns {

    class ServiceReceiver : public ::openscreen::discovery::DnsSdServiceWatcher<ServiceInfo> {
    public:
        explicit ServiceReceiver(
            openscreen::discovery::DnsSdService* service, std::string_view service_name,
            openscreen::discovery::DnsSdServiceWatcher<ServiceInfo>::ServicesUpdatedCallback cb);

        const std::string& service_name() const { return service_name_; }

    private:
        std::string service_name_;
    };  // ServiceReceiver

}  // namespace mdns