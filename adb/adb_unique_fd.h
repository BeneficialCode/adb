#pragma once

#include <errno.h>

#include "unique_fd.h"

// Helper to automatically close an FD when it goes out of scope.
struct AdbCloser {
    static void Close(int fd);
};

using unique_fd = android::base::unique_fd_impl<AdbCloser>;

using android::base::borrowed_fd;

template <typename T>
int adb_close(const android::base::unique_fd_impl<T>&);