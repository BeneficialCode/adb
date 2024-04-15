#pragma once

#include "adb_unique_fd.h"

#include <functional>

constexpr char kShellServiceArgRaw[] = "raw";
constexpr char kShellServiceArgPty[] = "pty";
constexpr char kShellServiceArgShellProtocol[] = "v2";

// Special flags sent by minadbd. They indicate the end of sideload transfer and the result of
// installation or wipe.
constexpr char kMinadbdServicesExitSuccess[] = "DONEDONE";
constexpr char kMinadbdServicesExitFailure[] = "FAILFAIL";

unique_fd create_service_thread(const char* service_name, std::function<void(unique_fd)> func);