#pragma once
#include "adb.h"

#include <string>

#include "macros.h"

// error/status codes for install_listener.
enum InstallStatus {
	INSTALL_STATUS_OK = 0,
	INSTALL_STATUS_INTERNAL_ERROR = -1,
	INSTALL_STATUS_CANNOT_BIND = -2,
	INSTALL_STATUS_CANNOT_REBIND = -3,
	INSTALL_STATUS_LISTENER_NOT_FOUND = -4,
};

inline constexpr int INSTALL_LISTENER_NO_REBIND = 1 << 0;
inline constexpr int INSTALL_LISTENER_DISABLED = 1 << 1;

InstallStatus install_listener(const std::string& local_name, const char* connect_to,
	atransport* transport, int flags, int* resolved_tcp_port,
	std::string* error);

std::string format_listeners();

InstallStatus remove_listener(const char* local_name, atransport* transport);
void remove_all_listeners(void);


void enable_server_sockets();
void close_smartsockets();