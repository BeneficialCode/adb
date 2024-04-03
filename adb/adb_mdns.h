#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <functional>

// The rules for Service Names [RFC6335] state that they may be no more
// than fifteen characters long (not counting the mandatory underscore),
// consisting of only letters, digits, and hyphens, must begin and end
// with a letter or digit, must not contain consecutive hyphens, and
// must contain at least one letter.
#define ADB_MDNS_SERVICE_TYPE "adb"
#define ADB_MDNS_TLS_PAIRING_TYPE "adb-tls-pairing"
#define ADB_MDNS_TLS_CONNECT_TYPE "adb-tls-connect"

// Client/service versions are initially defined to be matching,
// but may go out of sync as different clients and services
// try to talk to each other.
#define ADB_SECURE_SERVICE_VERSION 1
#define ADB_SECURE_CLIENT_VERSION ADB_SECURE_SERVICE_VERSION

constexpr int kADBTransportServiceRefIndex = 0;
constexpr int kADBSecurePairingServiceRefIndex = 1;
constexpr int kADBSecureConnectServiceRefIndex = 2;
constexpr int kNumADBDNSServices = 3;

extern const char* kADBSecurePairingServiceTxtRecord;
extern const char*  kADBSecureConnectServiceTxtRecord;

extern const char* kADBDNSServices[kNumADBDNSServices];
extern const char* kADBDNSServiceTxtRecords[kNumADBDNSServices];

// ADB Secure DNS service interface. Used to query what ADB Secure DNS services have been
// resolved, and to run some kind of callback for each one.
using adb_secure_foreach_service_callback =
std::function<void(const std::string& service_name, const std::string& reg_type,
    const std::string& ip_address, uint16_t port)>;

// Tries to connect to a |service_name| if found. Returns true if found and
// connected, false otherwise.
bool adb_secure_connect_by_service_name(const std::string& instance_name);

// Returns the index in kADBDNSServices array if |reg_type| matches a service name, otherwise
// std::nullopt.
std::optional<int> adb_DNSServiceIndexByName(std::string_view reg_type);
// Returns true if auto-connect is allowed for |service_name| and |instance_name|.
// See ADB_MDNS_AUTO_CONNECT environment variable for more info.
bool adb_DNSServiceShouldAutoConnect(std::string_view service_name, std::string_view instance_name);

void mdns_cleanup(void);
std::string mdns_check();
std::string mdns_list_discovered_services();

struct MdnsInfo {
    std::string service_name;
    std::string service_type;
    std::string addr;
    uint16_t port = 0;

    MdnsInfo(std::string_view name, std::string_view type, std::string_view addr, uint16_t port)
        : service_name(name), service_type(type), addr(addr), port(port) {}
};

std::optional<MdnsInfo> mdns_get_connect_service_info(const std::string& name);
std::optional<MdnsInfo> mdns_get_pairing_service_info(const std::string& name);

// TODO: Remove once openscreen has support for bonjour client APIs.
struct AdbMdnsResponderFuncs {
    std::string(* mdns_check)(void);
    std::string(* mdns_list_discovered_services)(void);
    std::optional<MdnsInfo>(* mdns_get_connect_service_info)(const std::string&);
    std::optional<MdnsInfo>(* mdns_get_pairing_service_info)(const std::string&);
    void (* mdns_cleanup)(void);
    bool (* adb_secure_connect_by_service_name)(const std::string&);
};  // AdbBonjourCallbacks

// TODO: Remove once openscreen has support for bonjour client APIs.
// Start mdns discovery using MdnsResponder backend. Fills in AdbMdnsResponderFuncs for adb mdns
// related functions.
AdbMdnsResponderFuncs StartMdnsResponderDiscovery();