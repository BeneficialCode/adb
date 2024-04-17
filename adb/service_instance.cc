#include "service_instance.h"

#include <utility>

#include "config.h"
#include "mdns_service.h"
#include "task_runner.h"

namespace openscreen {
    namespace discovery {

        ServiceInstance::ServiceInstance(TaskRunner* task_runner,
            ReportingClient* reporting_client,
            const Config& config,
            const InterfaceInfo& network_info)
            : task_runner_(task_runner),
            mdns_service_(MdnsService::Create(task_runner,
                reporting_client,
                config,
                network_info)),
            network_config_(network_info.index,
                network_info.GetIpAddressV4(),
                network_info.GetIpAddressV6()) {
            if (config.enable_querying) {
                querier_ = std::make_unique<QuerierImpl>(
                    mdns_service_.get(), task_runner_, reporting_client, &network_config_);
            }
            if (config.enable_publication) {
                publisher_ = std::make_unique<PublisherImpl>(
                    mdns_service_.get(), reporting_client, task_runner_, &network_config_);
            }
        }

        ServiceInstance::~ServiceInstance() {
            
        }

    }  // namespace discovery
}  // namespace openscreen