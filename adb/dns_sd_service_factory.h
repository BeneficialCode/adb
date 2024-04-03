#pragma once

#include "dns_sd_service.h"
#include "serial_delete_ptr.h"

namespace openscreen {

    class TaskRunner;

    namespace discovery {

        struct Config;
        class ReportingClient;

        SerialDeletePtr<DnsSdService> CreateDnsSdService(
            TaskRunner* task_runner,
            ReportingClient* reporting_client,
            const Config& config);

    }  // namespace discovery
}  // namespace openscreen