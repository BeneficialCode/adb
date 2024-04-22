#pragma once

#include <string>
#include <vector>

#include "file_sync_protocol.h"

bool do_sync_ls(const char* path);
bool do_sync_push(const std::vector<const char*>& srcs, const char* dst, bool sync,
    CompressionType compression, bool dry_run);
bool do_sync_pull(const std::vector<const char*>& srcs, const char* dst, bool copy_attrs,
    CompressionType compression, const char* name = nullptr);

bool do_sync_sync(const std::string& lpath, const std::string& rpath, bool list_only,
    CompressionType compression, bool dry_run);