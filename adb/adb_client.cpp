#define TRACE_TAG ADB

#include "sysdeps.h"
#include "adb_client.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <condition_variable>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "file.h"
#include "no_destructor.h"
#include "stringprintf.h"
#include "strings.h"
#include "thread_annotations.h"
#include "sockets.h"

#include "adb_io.h"
#include "adb_utils.h"
#include "socket_spec.h"
#include "chrono.h"

static TransportType __adb_transport = kTransportAny;
static const char* __adb_serial = nullptr;
static TransportId __adb_transport_id = 0;

static const char* __adb_server_socket_spec;
static const char* __adb_client_one_device;

void adb_set_transport(TransportType type, const char* serial, TransportId transport_id) {
    __adb_transport = type;
    __adb_serial = serial;
    __adb_transport_id = transport_id;
}

void adb_get_transport(TransportType* type, const char** serial, TransportId* transport_id) {
    if (type) *type = __adb_transport;
    if (serial) *serial = __adb_serial;
    if (transport_id) *transport_id = __adb_transport_id;
}

void adb_set_socket_spec(const char* socket_spec) {
    if (__adb_server_socket_spec) {
        LOG(FATAL) << "attempted to reinitialize adb_server_socket_spec " << socket_spec << " (was " << __adb_server_socket_spec << ")";
    }
    __adb_server_socket_spec = socket_spec;
}

void adb_set_one_device(const char* one_device) {
    __adb_client_one_device = one_device;
}

static std::optional<TransportId> switch_socket_transport(int fd, std::string* error) {
    TransportId result;
    bool read_transport = true;

    std::string service;
    if (__adb_transport_id) {
        read_transport = false;
        service += "host:transport-id:";
        service += std::to_string(__adb_transport_id);
        result = __adb_transport_id;
    }
    else if (__adb_serial) {
        service += "host:tport:serial:";
        service += __adb_serial;
    }
    else {
        const char* transport_type = "???";
        switch (__adb_transport) {
        case kTransportUsb:
            transport_type = "usb";
            break;
        case kTransportLocal:
            transport_type = "local";
            break;
        case kTransportAny:
            transport_type = "any";
            break;
        case kTransportHost:
            // no switch necessary
            return 0;
        }
        service += "host:tport:";
        service += transport_type;
    }

    if (!SendProtocolString(fd, service)) {
        *error = perror_str("write failure during connection");
        return std::nullopt;
    }

    LOG(DEBUG) << "Switch transport in progress: " << service;

    if (!adb_status(fd, error)) {
        D("Switch transport failed: %s", error->c_str());
        return std::nullopt;
    }

    if (read_transport) {
        if (!ReadFdExactly(fd, &result, sizeof(result))) {
            *error = "failed to read transport id from server";
            return std::nullopt;
        }
    }

    D("Switch transport success");
    return result;
}

bool adb_status(borrowed_fd fd, std::string* error) {
    char buf[5];
    if (!ReadFdExactly(fd, buf, 4)) {
        *error = perror_str("protocol fault (couldn't read status)");
        return false;
    }

    if (!memcmp(buf, "OKAY", 4)) {
        return true;
    }

    if (memcmp(buf, "FAIL", 4)) {
        *error = android::base::StringPrintf("protocol fault (status %02x %02x %02x %02x?!)",
            buf[0], buf[1], buf[2], buf[3]);
        return false;
    }

    ReadProtocolString(fd, error, error);
    return false;
}

static int _adb_connect(std::string_view service, TransportId* transport, std::string* error,
    bool force_switch = false) {
    LOG(DEBUG) << "_adb_connect: " << service;
    if (service.empty() || service.size() > MAX_PAYLOAD) {
        *error = android::base::StringPrintf("bad service name length (%zd)", service.size());
        return -1;
    }

    std::string reason;
    unique_fd fd;
    if (!socket_spec_connect(&fd, __adb_server_socket_spec, nullptr, nullptr, &reason)) {
        *error = android::base::StringPrintf("cannot connect to daemon at %s: %s",
            __adb_server_socket_spec, reason.c_str());
        return -2;
    }

    if (!service.starts_with("host") || force_switch) {
        std::optional<TransportId> transport_result = switch_socket_transport(fd.get(), error);
        if (!transport_result) {
            return -1;
        }

        if (transport) {
            *transport = *transport_result;
        }
    }

    if (!SendProtocolString(fd.get(), service)) {
        *error = perror_str("write failure during connection");
        return -1;
    }

    if (!adb_status(fd.get(), error)) {
        return -1;
    }

    D("_adb_connect: return fd %d", fd.get());
    return fd.release();
}

bool adb_kill_server() {
    D("adb_kill_server");
    std::string reason;
    unique_fd fd;
    if (!socket_spec_connect(&fd, __adb_server_socket_spec, nullptr, nullptr, &reason)) {
        fprintf(stderr, "cannot connect to daemon at %s: %s\n", __adb_server_socket_spec,
            reason.c_str());
        return true;
    }

    if (!SendProtocolString(fd.get(), "host:kill")) {
        char error[256];
        strerror_s(error, sizeof(error), errno);
        fprintf(stderr, "error: write failure during connection: %s\n", error);
        return false;
    }

    char buf[4];
    if (!ReadFdExactly(fd.get(), buf, 4)) {
        fprintf(stderr, "error: failed to read response from server\n");
        return false;
    }

    if (memcmp(buf, "OKAY", 4) == 0) {
        // Nothing to do.
    }
    else if (memcmp(buf, "FAIL", 4) == 0) {
        std::string output, error;
        if (!ReadProtocolString(fd.get(), &output, &error)) {
            fprintf(stderr, "error: %s\n", error.c_str());
            return false;
        }

        fprintf(stderr, "error: %s\n", output.c_str());
        return false;
    }

    // Now that no more data is expected, wait for socket orderly shutdown or error, indicating
    // server death.
    ReadOrderlyShutdown(fd.get());
    return true;
}

int adb_connect(std::string_view service, std::string* error) {
    return adb_connect(nullptr, service, error);
}

#if defined(__linux__)

#endif

static bool __adb_check_server_version(std::string* error) {
    unique_fd fd(_adb_connect("host:version", nullptr, error));

    bool local = is_local_socket_spec(__adb_server_socket_spec);
    if (fd == -2 && !local) {
        fprintf(stderr, "* cannot start server on remote host\n");
        // error is the original network connection error
        return false;
    }
    else if (fd == -2) {
        fprintf(stderr, "* daemon not running; starting now at %s\n", __adb_server_socket_spec);
    start_server:
        if (launch_server(__adb_server_socket_spec, __adb_client_one_device)) {
            fprintf(stderr, "* failed to start daemon\n");
            // launch_server() has already printed detailed error info, so just
            // return a generic error string about the overall adb_connect()
            // that the caller requested.
            *error = "cannot connect to daemon";
            return false;
        }
        else {
            fprintf(stderr, "* daemon started successfully\n");
        }
        // The server will wait until it detects all of its connected devices before acking.
        // Fall through to _adb_connect.
    }
    else {
        // If a server is already running, check its version matches.
        int version = 0;

        // If we have a file descriptor, then parse version result.
        if (fd >= 0) {
            std::string version_string;
            if (!ReadProtocolString(fd, &version_string, error)) {
                return false;
            }

            ReadOrderlyShutdown(fd);

            if (sscanf_s(&version_string[0], "%04x", &version) != 1) {
                *error = android::base::StringPrintf("cannot parse version string: %s",
                    version_string.c_str());
                return false;
            }
        }
        else {
            // If fd is -1 check for "unknown host service" which would
            // indicate a version of adb that does not support the
            // version command, in which case we should fall-through to kill it.
            if (*error != "unknown host service") {
                return false;
            }
        }

        if (version != ADB_SERVER_VERSION) {
#if defined(__linux__)
        
#endif

            fprintf(stderr, "adb server version (%d) doesn't match this client (%d); killing...\n",
                version, ADB_SERVER_VERSION);
            adb_kill_server();
            goto start_server;
        }
    }

    return true;
}

bool adb_check_server_version(std::string* error) {
    // Only check the version once per process, since this isn't atomic anyway.
    static std::once_flag once;
    static bool result;
    static std::string* err;
    std::call_once(once, []() {
        err = new std::string();
        result = __adb_check_server_version(err);
        });
    *error = *err;
    return result;
}

int adb_connect(TransportId* transport, std::string_view service, std::string* error,
    bool force_switch_device) {
    LOG(DEBUG) << "adb_connect: service: " << service;

    // Query the adb server's version.
    if (!adb_check_server_version(error)) {
        return -1;
    }

    // if the command is start-server, we are done.
    if (service == "host:start-server") {
        return 0;
    }

    unique_fd fd(_adb_connect(service, transport, error, force_switch_device));
    if (fd == -1) {
        D("_adb_connect error: %s", error->c_str());
    }
    else if (fd == -2) {
        fprintf(stderr, "* daemon still not running\n");
    }
    D("adb_connect: return fd %d", fd.get());

    return fd.release();
}

bool adb_command(const std::string& service) {
    std::string error;
    unique_fd fd(adb_connect(service, &error));
    if (fd < 0) {
        fprintf(stderr, "error: %s\n", error.c_str());
        return false;
    }

    if (!adb_status(fd.get(), &error)) {
        fprintf(stderr, "error: %s\n", error.c_str());
        return false;
    }

    ReadOrderlyShutdown(fd.get());
    return true;
}

bool adb_query(const std::string& service, std::string* result, std::string* error,
    bool force_switch_device) {
    D("adb_query: %s", service.c_str());
    unique_fd fd(adb_connect(nullptr, service, error, force_switch_device));
    if (fd < 0) {
        return false;
    }

    result->clear();
    if (!ReadProtocolString(fd.get(), result, error)) {
        return false;
    }

    ReadOrderlyShutdown(fd.get());
    return true;
}

std::string format_host_command(const char* command) {
    if (__adb_transport_id) {
        return android::base::StringPrintf("host-transport-id:%" PRIu64 ":%s", __adb_transport_id,
            command);
    }
    else if (__adb_serial) {
        return android::base::StringPrintf("host-serial:%s:%s", __adb_serial, command);
    }

    const char* prefix = "host";
    if (__adb_transport == kTransportUsb) {
        prefix = "host-usb";
    }
    else if (__adb_transport == kTransportLocal) {
        prefix = "host-local";
    }
    return android::base::StringPrintf("%s:%s", prefix, command);
}

const std::optional<FeatureSet>& adb_get_feature_set(std::string* error) {
    static std::mutex feature_mutex;
    _Guarded_by_(feature_mutex) static std::optional<FeatureSet> features;
    std::lock_guard<std::mutex> lock(feature_mutex);
    if (!features) {
        std::string result;
        std::string err;
        if (adb_query(format_host_command("features"), &result, &err)) {
            features = StringToFeatureSet(result);
        }
        else {
            if (error) {
                *error = err;
            }
        }
    }
    return features;
}