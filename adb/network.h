#pragma once

#include <string>

int network_loopback_client(int port, int type, std::string* error);
int network_loopback_server(int port, int type, std::string* error, bool prefer_ipv4);