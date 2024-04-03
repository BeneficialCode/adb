#pragma once

#include <string>
#include "macros.h"

class TemporaryFile {
public:
	TemporaryFile();
	~TemporaryFile();
	int fd;
	char path[1024];
private:
	void init(const std::string& tmp_dir);
	DISALLOW_COPY_AND_ASSIGN(TemporaryFile);
};
class TemporaryDir {
public:
	TemporaryDir();
	~TemporaryDir();
	char path[1024];
private:
	bool init(const std::string& tmp_dir);
	DISALLOW_COPY_AND_ASSIGN(TemporaryDir);
};