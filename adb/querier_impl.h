#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <absl/hash/hash.h>
#include <absl/strings/string_view.h>
#include "constants.h"
#include "conversion_layer.h"
#include "dns_data_graph.h"
#include "instance_key.h"
#include "service_key.h"
#include "dns_sd_instance_endpoint.h"
#include "dns_sd_querier.h"
#include "mdns_record_changed_callback.h"
#include "mdns_records.h"
#include "mdns_service.h"

namespace openscreen {
    namespace discovery {

        class NetworkInterfaceConfig;
        class ReportingClient;

        class QuerierImpl : public DnsSdQuerier, public MdnsRecordChangedCallback {
        public:
            // |querier|, |task_runner|, and |network_config| must outlive the QuerierImpl
            // instance constructed.
            QuerierImpl(MdnsService* querier,
                TaskRunner* task_runner,
                ReportingClient* reporting_client,
                const NetworkInterfaceConfig* network_config);
            ~QuerierImpl() override;

            bool IsQueryRunning(const std::string& service) const;

            // DnsSdQuerier overrides.
            void StartQuery(const std::string& service, Callback* callback) override;
            void StopQuery(const std::string& service, Callback* callback) override;
            void ReinitializeQueries(const std::string& service) override;

            // MdnsRecordChangedCallback overrides.
            std::vector<PendingQueryChange> OnRecordChanged(
                const MdnsRecord& record,
                RecordChangedEvent event) override;

        private:
            friend class QuerierImplTesting;

            // Applies the provided record change to the underlying |graph_| instance.
            ErrorOr<std::vector<PendingQueryChange>> ApplyRecordChanges(
                const MdnsRecord& record,
                RecordChangedEvent event);

            // Informs all relevant callbacks of the provided changes.
            void InvokeChangeCallbacks(std::vector<DnsSdInstanceEndpoint> created,
                std::vector<DnsSdInstanceEndpoint> updated,
                std::vector<DnsSdInstanceEndpoint> deleted);

            // Graph of underlying mDNS Record and their associations with each-other.
            std::unique_ptr<DnsDataGraph> graph_;

            // Map from the (service, domain) pairs currently being queried for to the
            // callbacks to call when new InstanceEndpoints are available.
            std::map<ServiceKey, std::vector<Callback*>> callback_map_;

            MdnsService* const mdns_querier_;
            TaskRunner* const task_runner_;

            ReportingClient* reporting_client_;
        };

    }  // namespace discovery
}  // namespace openscreen
