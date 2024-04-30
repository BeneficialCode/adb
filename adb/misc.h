#pragma once

#include <sys/types.h>

//
// Handy utility functions and portability code.
//

namespace android {

    /*
     * Some utility functions for working with files.  These could be made
     * part of a "File" class.
     */
    typedef enum FileType {
        kFileTypeUnknown = 0,
        kFileTypeNonexistent,       // i.e. ENOENT
        kFileTypeRegular,
        kFileTypeDirectory,
        kFileTypeCharDev,
        kFileTypeBlockDev,
        kFileTypeFifo,
        kFileTypeSymlink,
        kFileTypeSocket,
    } FileType;
    /* get the file's type; follows symlinks */
    FileType getFileType(const char* fileName);
    /* get the file's modification date; returns -1 w/errno set on failure */
    time_t getFileModDate(const char* fileName);

    // Check if |path| or |fd| resides on a readonly filesystem.
    bool isReadonlyFilesystem(const char* path);
    bool isReadonlyFilesystem(int fd);

}; // namespace android