#pragma once

#include <vector>

int capture_shell_command(const char* command, std::vector<char>* outBuffer,
    std::vector<char>* errBuffer);