#pragma once

#include <string>
#include <tuple>

#include "adb_unique_fd.h"

// Returns true if the argument starts with a plausible socket prefix.
bool is_socket_spec(std::string_view spec);
bool is_local_socket_spec(std::string_view spec);

bool socket_spec_connect(unique_fd* fd, std::string_view address, int* port, std::string* serial,
    std::string* error);
int socket_spec_listen(std::string_view spec, std::string* error, int* resolved_tcp_port = nullptr);

bool parse_tcp_socket_spec(std::string_view spec, std::string* hostname, int* port,
    std::string* serial, std::string* error);

int get_host_socket_spec_port(std::string_view spec, std::string* error);