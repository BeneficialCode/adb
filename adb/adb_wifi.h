#pragma once

#include <optional>
#include <string>

#include "adb.h"

void adb_wifi_init(void);
void adb_wifi_pair_device(const std::string& host, const std::string& password,
    std::string& response);
bool adb_wifi_is_known_host(const std::string& host);