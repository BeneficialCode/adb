#pragma once

#include <string>
#include <vector>

#include <absl/strings/string_view.h>
#include "constants.h"
#include "dns_sd_instance_endpoint.h"
#include "dns_sd_txt_record.h"
#include "mdns_records.h"
#include "error.h"

namespace openscreen {
	namespace discovery {

		class InstanceKey;
		class ServiceKey;

		// *** Conversions from DNS entities to DNS-SD Entities ***

		// Attempts to create a new TXT record from the provided set of strings,
		// returning a TxtRecord on success or an error if the provided strings are
		// not valid.
		ErrorOr<DnsSdTxtRecord> CreateFromDnsTxt(const TxtRecordRdata& txt);

		bool IsPtrRecord(const MdnsRecord& record);

		// Checks that the instance, service, and domain ids in this instance are valid.
		bool HasValidDnsRecordAddress(const MdnsRecord& record);
		bool HasValidDnsRecordAddress(const DomainName& domain);

		//*** Conversions to DNS entities from DNS-SD Entities ***

		// Returns the Domain Name associated with this InstanceKey.
		DomainName GetDomainName(const InstanceKey& key);

		// Returns the domain name associated with this MdnsRecord. In the case of a PTR
		// record, this is the target domain, and it is the named domain in all other
		// cases.
		DomainName GetDomainName(const MdnsRecord& record);

		// Returns the query required to get all instance information about the service
		// instances described by the provided InstanceKey.
		DnsQueryInfo GetInstanceQueryInfo(const InstanceKey& key);

		// Returns the query required to get all service information that matches the
		// provided key.
		DnsQueryInfo GetPtrQueryInfo(const ServiceKey& key);

		// Returns all MdnsRecord entities generated from this object.
		std::vector<MdnsRecord> GetDnsRecords(const DnsSdInstanceEndpoint& endpoint);
		std::vector<MdnsRecord> GetDnsRecords(const DnsSdInstance& instance);

	}  // namespace discovery
}  // namespace openscreen