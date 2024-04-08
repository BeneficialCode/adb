#pragma once

#include <functional>
#include <optional>
#include <string>

#include "adb.h"
#include "adb_unique_fd.h"
#include "sysdeps.h"
#include "transport.h"

// Explicitly check the adb server version.
// All of the commands below do this implicitly.
// Only the first invocation of this function will check the server version.
bool adb_check_server_version(std::string* error);

// Connect to adb, connect to the named service, and return a valid fd for
// interacting with that service upon success or a negative number on failure.
int adb_connect(std::string_view service, std::string* error);

// Same as above, except returning the TransportId for the service that we've connected to.
// force_switch_device forces the function to attempt to select a device, even if the service
// string appears to be a host: service (for use with host services that are device specific, like
// forward).
int adb_connect(TransportId* id, std::string_view service, std::string* error,
    bool force_switch_device = false);

// Kill the currently running adb server, if it exists.
bool adb_kill_server();

// Connect to adb, connect to the named service, returns true if the connection
// succeeded AND the service returned OKAY. Outputs any returned error otherwise.
bool adb_command(const std::string& service);

// Connects to the named adb service and fills 'result' with the response.
// Returns true on success; returns false and fills 'error' on failure.
bool adb_query(const std::string& service, std::string* result,
    std::string* error, bool force_switch_device = false);

// Set the preferred transport to connect to.
void adb_set_transport(TransportType type, const char* serial, TransportId transport_id);
void adb_get_transport(TransportType* type, const char* serial,
    TransportId* transport_id);

// Set the server's "one device" id. This is either null, or a pointer to a serial number
// or a pointer to a usb device path, the argument must live to the end of the process.
void adb_set_one_device(const char* one_device);

// Set the socket specification for the adb server.
// This function can only be called once, and the argument must live to the end of the process.
void adb_set_socket_spec(const char* socket_spec);

// Send commands to the current emulator instance. Will fail if there is not
// exactly one emulator connected (or if you use -s <serial> with a <serial>
// that does not designate an emulator).
int adb_send_emulator_command(int argc, const char* argv,
    const char* serial);

// Reads a standard adb status response (OKAY|FAIL) and returns true in the
// event of OKAY, false in the event of FAIL or protocol error.
bool adb_status(borrowed_fd fd, std::string* error);

// Create a host command corresponding to selected transport type/serial.
std::string format_host_command(const char* command);

// Get the feature set of the current preferred transport.
const std::optional<FeatureSet>& adb_get_feature_set(std::string* error);

#if defined(__linux__)

#endif

// Globally acccesible argv/envp, for the purpose of re-execing adb.
extern const char*  __adb_argv;
extern const char*  __adb_envp;