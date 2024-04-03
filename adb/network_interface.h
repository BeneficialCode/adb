#pragma once

#include <vector>

#include "interface_info.h"

namespace openscreen {

	// Returns an InterfaceInfo for each currently active network interface on the
	// system. No two entries in this vector can have the same NetworkInterfaceIndex
	// value.
	//
	// This can return an empty vector if: 1) there are no active network
	// interfaces, 2) an error occurred querying the system for them, or 3) Open
	// Screen features that involve broadcast listening/publishing (e.g., mDNS
	// discovery) are not being used.
	std::vector<InterfaceInfo> GetNetworkInterfaces();

}  // namespace openscreen