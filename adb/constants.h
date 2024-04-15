#pragma once

#include <string>
#include <utility>

#include "mdns_records.h"
#include "mdns_constants.h"

namespace openscreen {
	namespace discovery {

		// This is the DNS Information required to start a new query.
		struct DnsQueryInfo {
			DomainName name;
			DnsType dns_type;
			DnsClass dns_class;
		};

	}  // namespace discovery
}  // namespace openscreen