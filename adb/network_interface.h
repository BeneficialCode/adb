#pragma once

#include <vector>

#include <absl/types/optional.h>

#include "interface_info.h"

namespace openscreen {

	// The below functions are responsible for returning the network interfaces
	// provided of the current machine. GetAllInterfaces() returns all interfaces,
	// real or virtual. GetLoopbackInterfaceForTesting() returns one such interface
	// which is associated with the machine's loopback interface, while
	// GetNetworkInterfaces() returns all non-loopback interfaces.
	std::vector<InterfaceInfo> GetAllInterfaces();
	absl::optional<InterfaceInfo> GetLoopbackInterfaceForTesting();
	std::vector<InterfaceInfo> GetNetworkInterfaces();

}  // namespace openscreen