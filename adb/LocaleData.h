#pragma once

#include <stddef.h>
#include <stdint.h>

namespace android {

    int localeDataCompareRegions(
        const char* left_region, const char* right_region,
        const char* requested_language, const char* requested_script,
        const char* requested_region);

    void localeDataComputeScript(char out[4], const char* language, const char* region);

    bool localeDataIsCloseToUsEnglish(const char* region);

} // namespace android