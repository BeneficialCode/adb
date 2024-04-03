#pragma once

#include <sys/types.h>

#include "adb_unique_fd.h"

// Layout of this struct must match struct WSABUF (verified via static assert in sysdeps_win32.cpp)
struct adb_iovec {
    unsigned int iov_len;
    void* iov_base;
};

size_t adb_writev(borrowed_fd fd, const adb_iovec* iov, int iovcnt);