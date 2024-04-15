#pragma once

#include <absl/strings/string_view.h>
#include "conversion_layer.h"
#include "dns_sd_instance.h"
#include "dns_sd_publisher.h"
#include "mdns_domain_confirmed_provider.h"
#include "mdns_service.h"

namespace openscreen {
    namespace discovery {

        class NetworkInterfaceConfig;
        class ReportingClient;

        class PublisherImpl : public DnsSdPublisher,
            public MdnsDomainConfirmedProvider {
        public:
            PublisherImpl(MdnsService* publisher,
                ReportingClient* reporting_client,
                TaskRunner* task_runner,
                const NetworkInterfaceConfig* network_config);
            ~PublisherImpl() override;

            // DnsSdPublisher overrides.
            Error Register(const DnsSdInstance& instance, Client* client) override;
            Error UpdateRegistration(const DnsSdInstance& instance) override;
            ErrorOr<int> DeregisterAll(const std::string& service) override;

        private:
            Error UpdatePublishedRegistration(const DnsSdInstance& instance);

            // MdnsDomainConfirmedProvider overrides.
            void OnDomainFound(const DomainName& requested_name,
                const DomainName& confirmed_name) override;

            // The set of instances which will be published once the mDNS Probe phase
            // completes.
            std::map<DnsSdInstance, Client* const> pending_instances_;

            // Maps from the requested instance to the endpoint which was published after
            // the mDNS Probe phase was completed. The only difference between these
            // instances should be the instance name.
            std::map<DnsSdInstance, DnsSdInstanceEndpoint> published_instances_;

            MdnsService* const mdns_publisher_;
            ReportingClient* const reporting_client_;
            TaskRunner* const task_runner_;
            const NetworkInterfaceConfig* const network_config_;

            friend class PublisherTesting;
        };

    }  // namespace discovery
}  // namespace openscreen