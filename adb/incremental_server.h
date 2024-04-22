#pragma once

namespace incremental {

	// Expecting arguments like:
	// {FILE1 FILE2 ...}
	// Where FILE* are files to serve.
	bool serve(int connection_fd, int output_fd, int argc, const char** argv);

}  // namespace incremental