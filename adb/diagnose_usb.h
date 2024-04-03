#pragma once

#include <string>

// USB permission error help text. The short version will be one line, long may be multi-line.
// Returns a string message to print, or an empty string if no problems could be found.
std::string UsbNoPermissionsShortHelpText();
std::string UsbNoPermissionsLongHelpText();