#pragma once

#include <vector>

#include "mdns_records.h"

namespace openscreen {
    namespace discovery {

        enum class RecordChangedEvent {
            kCreated,
            kUpdated,
            kExpired,
        };

        class MdnsRecordChangedCallback;

        struct PendingQueryChange {
            enum ChangeType { kStartQuery, kStopQuery };
            DomainName name;
            DnsType dns_type;
            DnsClass dns_class;
            MdnsRecordChangedCallback* callback;
            ChangeType change_type;
        };

        class MdnsRecordChangedCallback {
        public:
            virtual ~MdnsRecordChangedCallback() = default;

            // Called when |record| has been changed.
            // NOTE: This callback may not modify the instance from which it is called.
            // The return value of this function must be the set of all record changes to
            // be made once the operation completes.
            virtual std::vector<PendingQueryChange> OnRecordChanged(
                const MdnsRecord& record,
                RecordChangedEvent event) = 0;
        };

        inline std::ostream& operator<<(std::ostream& output,
            RecordChangedEvent event) {
            switch (event) {
            case RecordChangedEvent::kCreated:
                return output << "Create";
            case RecordChangedEvent::kUpdated:
                return output << "Update";
            case RecordChangedEvent::kExpired:
                return output << "Expiry";
            }
        }

    }  // namespace discovery
}  // namespace openscreen