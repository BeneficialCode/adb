#include "diagnose_usb.h"

#include <errno.h>

#include <algorithm>
#include <string>
#include <vector>

#include "stringprintf.h"

static const char kPermissionsHelpUrl[] = "http://developer.android.com/tools/device.html";

// Returns a message describing any potential problems we find with udev, or an empty string if we
// can't find plugdev information (i.e. udev is not installed).
static std::string GetUdevProblem() {
#if defined(__linux__) && !defined(__BIONIC__)
    
#else
    return "";
#endif
}

// Short help text must be a single line, and will look something like:
//
//   no permissions (reason); see [URL]
std::string UsbNoPermissionsShortHelpText() {
    std::string help_text = "no permissions";

    std::string problem(GetUdevProblem());
    if (!problem.empty()) help_text += " (" + problem + ")";

    return android::base::StringPrintf("%s; see [%s]", help_text.c_str(), kPermissionsHelpUrl);
}

// Long help text can span multiple lines but doesn't currently provide more detailed information:
//
//   insufficient permissions for device: reason
//   See [URL] for more information
std::string UsbNoPermissionsLongHelpText() {
    std::string header = "insufficient permissions for device";

    std::string problem(GetUdevProblem());
    if (!problem.empty()) header += ": " + problem;

    return android::base::StringPrintf("%s\nSee [%s] for more information", header.c_str(),
        kPermissionsHelpUrl);
}