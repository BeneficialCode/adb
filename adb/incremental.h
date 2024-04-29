#pragma once

#include "adb_unique_fd.h"

#include <optional>
#include <string>

#include "sysdeps.h"

namespace incremental {

	using Files = std::vector<std::string>;
	using Args = std::vector<std::string_view>;

	bool can_install(const Files& files);
	std::optional<Process> install(const Files& files, const Args& passthrough_args, bool silent);

	enum class Result { Success, Failure, None };
	Result wait_for_installation(int read_fd);

}  // namespace incremental