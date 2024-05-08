#pragma once

namespace incfs::util {

    // Some tools useful for writing hardened code

    // Clear the passed container and make sure it frees all allocated memory.
    // Useful for signal handling code where any remaining memory would leak.
    template <class Container>
    void clearAndFree(Container& c) {
        Container().swap(c);
    }

    bool isIncfsFd([[maybe_unused]] int fd) {
#ifdef __BIONIC__
        struct statfs fs = {};
        if (::fstatfs(fd, &fs) != 0) {
            return false;
        }
        return fs.f_type == static_cast<decltype(fs.f_type)>(INCFS_MAGIC_NUMBER);
#else
        return false;  // incfs is linux-specific, and only matters on Android.
#endif
    }

}  // namespace incfs::util