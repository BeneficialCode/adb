#pragma once

#include <functional>
#include <memory>

#include "error.h"
#include "interface_info.h"
#include "ip_address.h"


namespace openscreen {

	struct IPEndpoint;
	class TaskRunner;

	namespace discovery {

		struct Config;
		class DnsSdPublisher;
		class DnsSdQuerier;
		class ReportingClient;

		// This class provides a wrapper around DnsSdQuerier and DnsSdPublisher to
		// allow for an embedder-overridable factory method below.
		class DnsSdService {
		public:
			virtual ~DnsSdService() = default;

			// Returns the DnsSdQuerier owned by this DnsSdService. If queries are not
			// supported, returns nullptr.
			virtual DnsSdQuerier* GetQuerier() = 0;

			// Returns the DnsSdPublisher owned by this DnsSdService. If publishing is not
			// supported, returns nullptr.
			virtual DnsSdPublisher* GetPublisher() = 0;
		};

	}  // namespace discovery
}  // namespace openscreen