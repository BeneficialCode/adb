#pragma once

#include <string>

int install_app(int argc, const char** argv);
int install_multiple_app(int argc, const char** argv);
int install_multi_package(int argc, const char** argv);
int uninstall_app(int argc, const char** argv);

int delete_device_file(const std::string& filename);
int delete_host_file(const std::string& filename);