#pragma once

#include <memory>
#include "querier_impl.h"
#include "service_instance.h"
#include "dns_sd_querier.h"
#include "dns_sd_service.h"

namespace openscreen {

    class TaskRunner;

    namespace discovery {

        struct Config;
        class ReportingClient;

        class ServiceDispatcher final : public DnsSdPublisher,
            public DnsSdQuerier,
            public DnsSdService {
        public:
            ServiceDispatcher(TaskRunner* task_runner,
                ReportingClient* reporting_client,
                const Config& config);
            ~ServiceDispatcher() override;

            // DnsSdService overrides.
            DnsSdQuerier* GetQuerier() override { return querier_; }
            DnsSdPublisher* GetPublisher() override { return publisher_; }

        private:
            // DnsSdQuerier overrides.
            void StartQuery(const std::string& service, Callback* cb) override;
            void StopQuery(const std::string& service, Callback* cb) override;
            void ReinitializeQueries(const std::string& service) override;

            // DnsSdPublisher overrides.
            Error Register(const DnsSdInstance& instance, Client* client) override;
            Error UpdateRegistration(const DnsSdInstance& instance) override;
            ErrorOr<int> DeregisterAll(const std::string& service) override;

            std::vector<std::unique_ptr<ServiceInstance>> service_instances_;

            TaskRunner* const task_runner_;

            // Pointers either to this instance or to nullptr depending whether the below
            // types are supported.
            DnsSdPublisher* const publisher_;
            DnsSdQuerier* const querier_;
        };

    }  // namespace discovery
}  // namespace openscreen