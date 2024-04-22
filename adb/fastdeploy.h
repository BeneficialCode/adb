#pragma once

#include "adb_unique_fd.h"

#include "ApkEntry.pb.h"

#include <optional>
#include <string>

enum FastDeploy_AgentUpdateStrategy {
    FastDeploy_AgentUpdateAlways,
    FastDeploy_AgentUpdateNewerTimeStamp,
    FastDeploy_AgentUpdateDifferentVersion
};

void fastdeploy_set_agent_update_strategy(FastDeploy_AgentUpdateStrategy agent_update_strategy);
int get_device_api_level();

std::optional<com::android::fastdeploy::APKMetaData> extract_metadata(const char* apk_path);
unique_fd install_patch(int argc, const char** argv);
unique_fd apply_patch_on_device(const char* output_path);
int stream_patch(const char* apk_path, com::android::fastdeploy::APKMetaData metadata,
    unique_fd patch_fd);