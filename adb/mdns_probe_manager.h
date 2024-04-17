#pragma once

#include "mdns_records.h"


namespace openscreen {
    namespace discovery {

        class MdnsDomainConfirmedProvider {
        public:
            virtual ~MdnsDomainConfirmedProvider() = default;

            // Called once the probing phase has been completed, and a DomainName has
            // been confirmed. The callee is expected to register records for the
            // newly confirmed name in this callback. Note that the requested name and
            // the confirmed name may differ if conflict resolution has occurred.
            virtual void OnDomainFound(const DomainName& requested_name,
                const DomainName& confirmed_name) = 0;
        };

    }  // namespace discovery
}  // namespace openscreen