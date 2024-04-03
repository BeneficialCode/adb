#include "stat.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string>

#include "utf8.h"

// Version of stat() that takes a UTF-8 path.
int adb_stat(const char* path, struct adb_stat* s) {
    // This definition of wstat seems to be missing from <sys/stat.h>.
#if defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)
#ifdef _USE_32BIT_TIME_T
#define wstat _wstat32i64
#else
#define wstat _wstat64
#endif
#else
// <sys/stat.h> has a function prototype for wstat() that should be available.
#endif

#ifndef _S_ISTYPE
#define _S_ISTYPE(mode, mask)  (((mode) & _S_IFMT) == (mask))
#define S_ISREG(mode) _S_ISTYPE((mode), _S_IFREG)
#define S_ISDIR(mode) _S_ISTYPE((mode), _S_IFDIR)
#endif

    std::wstring path_wide;
    if (!android::base::UTF8ToWide(path, &path_wide)) {
        errno = ENOENT;
        return -1;
    }

    // If the path has a trailing slash, stat will fail with ENOENT regardless of whether the path
    // is a directory or not.
    bool expected_directory = false;
    while (*path_wide.rbegin() == u'/' || *path_wide.rbegin() == u'\\') {
        path_wide.pop_back();
        expected_directory = true;
    }

    struct _stat st;
    int result = _wstat(path_wide.c_str(), &st);
    if (result == 0 && expected_directory) {
        if (!S_ISDIR(st.st_mode)) {
            errno = ENOTDIR;
            return -1;
        }
    }

    memcpy(s, &st, sizeof(st));
    return result;
}