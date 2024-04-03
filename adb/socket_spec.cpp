#include "socket_spec.h"

#include <limits>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "parseint.h"
#include "parsenetaddress.h"
#include "stringprintf.h"
#include "strings.h"
#include "sockets.h"

#include "adb.h"
#include "adb_auth.h"
#include "adb_mdns.h"
#include "adb_utils.h"
#include "sysdeps.h"

using namespace std::string_literals;

using android::base::ConsumePrefix;
using android::base::StringPrintf;

#if defined(__linux__)
#define ADB_LINUX 1
#else
#define ADB_LINUX 0
#endif

#if defined(_WIN32)
#define ADB_WINDOWS 1
#else
#define ADB_WINDOWS 0
#endif

// Not static because it is used in commandline.c.
int gListenAll = 0;

struct LocalSocketType {
    int socket_namespace;
    bool available;
};

static auto& kLocalSocketTypes = *new std::unordered_map<std::string, LocalSocketType>({
#if ADB_HOST
    { "local", { ANDROID_SOCKET_NAMESPACE_FILESYSTEM, !ADB_WINDOWS } },
#else
    { "local", { ANDROID_SOCKET_NAMESPACE_RESERVED, !ADB_WINDOWS } },
#endif

    { "localreserved", { ANDROID_SOCKET_NAMESPACE_RESERVED, !ADB_HOST } },
    { "localabstract", { ANDROID_SOCKET_NAMESPACE_ABSTRACT, ADB_LINUX } },
    { "localfilesystem", { ANDROID_SOCKET_NAMESPACE_FILESYSTEM, !ADB_WINDOWS } },
    });

bool parse_tcp_socket_spec(std::string_view spec, std::string* hostname, int* port,
    std::string* serial, std::string* error) {
    if (!spec.starts_with("tcp:")) {
        *error = "specification is not tcp: ";
        *error += spec;
        return false;
    }

    std::string hostname_value;
    int port_value;

    // If the spec is tcp:<port>, parse it ourselves.
    // Otherwise, delegate to android::base::ParseNetAddress.
    if (android::base::ParseInt(&spec[4], &port_value)) {
        // Do the range checking ourselves, because ParseInt rejects 'tcp:65536' and 'tcp:foo:1234'
        // identically.
        if (port_value < 0 || port_value > 65535) {
            *error = StringPrintf("bad port number '%d'", port_value);
            return false;
        }
    }
    else {
        std::string addr(spec.substr(4));
        port_value = DEFAULT_ADB_LOCAL_TRANSPORT_PORT;

        // FIXME: ParseNetAddress rejects port 0. This currently doesn't hurt, because listening
        //        on an address that isn't 'localhost' is unsupported.
        if (!android::base::ParseNetAddress(addr, &hostname_value, &port_value, serial, error)) {
            return false;
        }
    }

    if (hostname) {
        *hostname = std::move(hostname_value);
    }

    if (port) {
        *port = port_value;
    }

    return true;
}

int get_host_socket_spec_port(std::string_view spec, std::string* error) {
    int port;
    if (spec.starts_with("tcp:")) {
        if (!parse_tcp_socket_spec(spec, nullptr, &port, nullptr, error)) {
            return -1;
        }
    }
    else if (spec.starts_with("vsock:")) {
#if ADB_LINUX
       
#else   // ADB_LINUX
        * error = "vsock is only supported on linux";
        return -1;
#endif  // ADB_LINUX
    }
    else {
        *error = "given socket spec string was invalid";
        return -1;
    }
    return port;
}

static bool tcp_host_is_local(std::string_view hostname) {
    // FIXME
    return hostname.empty() || hostname == "localhost";
}

bool is_socket_spec(std::string_view spec) {
    for (const auto& it : kLocalSocketTypes) {
        std::string prefix = it.first + ":";
        if (spec.starts_with(prefix)) {
            return true;
        }
    }
    return spec.starts_with("tcp:") || spec.starts_with("acceptfd:") || spec.starts_with("vsock:");
}

bool is_local_socket_spec(std::string_view spec) {
    for (const auto& it : kLocalSocketTypes) {
        std::string prefix = it.first + ":";
        if (spec.starts_with(prefix)) {
            return true;
        }
    }

    std::string error;
    std::string hostname;
    if (!parse_tcp_socket_spec(spec, &hostname, nullptr, nullptr, &error)) {
        return false;
    }
    return tcp_host_is_local(hostname);
}

bool socket_spec_connect(unique_fd* fd, std::string_view address, int* port, std::string* serial,
    std::string* error) {
#if !ADB_HOST
    
#endif

    if (address.starts_with("tcp:")) {
        std::string hostname;
        int port_value = port ? *port : 0;
        if (!parse_tcp_socket_spec(address, &hostname, &port_value, serial, error)) {
            return false;
        }

        if (tcp_host_is_local(hostname)) {
            fd->reset(network_loopback_client(port_value, SOCK_STREAM, error));
        }
        else {
#if ADB_HOST
            // Check if the address is an mdns service we can connect to.
            if (auto mdns_info = mdns_get_connect_service_info(std::string(address.substr(4)));
                mdns_info != std::nullopt) {
                fd->reset(network_connect(mdns_info->addr, mdns_info->port, SOCK_STREAM, 0, error));
                if (fd->get() != -1) {
                    // TODO(joshuaduong): We still show the ip address for the serial. Change it to
                    // use the mdns instance name, so we can adjust to address changes on
                    // reconnects.
                    port_value = mdns_info->port;
                    if (serial) {
                        *serial = android::base::StringPrintf("%s.%s",
                            mdns_info->service_name.c_str(),
                            mdns_info->service_type.c_str());
                    }
                }
            }
            else {
                fd->reset(network_connect(hostname, port_value, SOCK_STREAM, 0, error));
            }
#else
            // Disallow arbitrary connections in adbd.
            * error = "adbd does not support arbitrary tcp connections";
            return false;
#endif
        }

        if (fd->get() > 0) {
            int keepalive_interval = 1;
            if (const char* keepalive_env = getenv("ADB_TCP_KEEPALIVE_INTERVAL")) {
                android::base::ParseInt(keepalive_env, &keepalive_interval, 0);
            }

            set_tcp_keepalive(fd->get(), keepalive_interval);
            disable_tcp_nagle(fd->get());
            if (port) {
                *port = port_value;
            }
            return true;
        }
        return false;
    }
    else if (address.starts_with("vsock:")) {
#if ADB_LINUX
       
#else   // ADB_LINUX
        * error = "vsock is only supported on Linux";
        return false;
#endif  // ADB_LINUX
    }
    else if (address.starts_with("acceptfd:")) {
        *error = "cannot connect to acceptfd";
        return false;
    }

    for (const auto& it : kLocalSocketTypes) {
        std::string prefix = it.first + ":";
        if (address.starts_with(prefix)) {
            if (!it.second.available) {
                *error = StringPrintf("socket type %s is unavailable on this platform",
                    it.first.c_str());
                return false;
            }

            fd->reset(network_local_client(&address[prefix.length()], it.second.socket_namespace,
                SOCK_STREAM, error));

            if (fd->get() < 0) {
                *error =
                    android::base::StringPrintf("could not connect to %s address '%s'",
                        it.first.c_str(), std::string(address).c_str());
                return false;
            }

            if (serial) {
                *serial = address;
            }
            return true;
        }
    }

    *error = "unknown socket specification: ";
    *error += address;
    return false;
}

int socket_spec_listen(std::string_view spec, std::string* error, int* resolved_port) {
    if (spec.starts_with("tcp:")) {
        std::string hostname;
        int port;
        if (!parse_tcp_socket_spec(spec, &hostname, &port, nullptr, error)) {
            return -1;
        }

        int result;
#if ADB_HOST
        if (hostname.empty() && gListenAll) {
#else
        if (hostname.empty()) {
#endif
            result = network_inaddr_any_server(port, SOCK_STREAM, error);
        }
        else if (tcp_host_is_local(hostname)) {
            result = network_loopback_server(port, SOCK_STREAM, error, true);
        }
        else if (hostname == "::1") {
            result = network_loopback_server(port, SOCK_STREAM, error, false);
        }
        else {
            // TODO: Implement me.
            *error = "listening on specified hostname currently unsupported";
            return -1;
        }

        if (result >= 0 && resolved_port) {
            *resolved_port = adb_socket_get_local_port(result);
        }
        return result;
        }
    else if (spec.starts_with("vsock:")) {
#if ADB_LINUX
        
#else   // ADB_LINUX
        * error = "vsock is only supported on linux";
        return -1;
#endif  // ADB_LINUX
    }
    else if (ConsumePrefix(&spec, "acceptfd:")) {
#if ADB_WINDOWS
        * error = "socket activation not supported under Windows";
        return -1;
#else
     
#endif
    }

    for (const auto& it : kLocalSocketTypes) {
        std::string prefix = it.first + ":";
        if (spec.starts_with(prefix)) {
            if (!it.second.available) {
                *error = "attempted to listen on unavailable socket type: ";
                *error += spec;
                return -1;
            }

            return network_local_server(&spec[prefix.length()], it.second.socket_namespace,
                SOCK_STREAM, error);
        }
    }

    *error = "unknown socket specification:";
    *error += spec;
    return -1;
}
