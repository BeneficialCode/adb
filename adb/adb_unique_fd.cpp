#include "adb_unique_fd.h"

#include <errno.h>


#include "sysdeps.h"

void AdbCloser::Close(int fd) {
    adb_close(fd);
}