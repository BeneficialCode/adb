#pragma once

#include <sys/types.h>
#include <stdint.h>

#include "Compat.h"

#if defined(_WIN32)
// Ensure that we always pull in winsock2.h before windows.h
#if defined(_WIN32)
#include <winsock2.h>
#endif
#include <windows.h>
#endif

namespace android {

    /*
     * This represents a memory-mapped file.  It might be the entire file or
     * only part of it.  This requires a little bookkeeping because the mapping
     * needs to be aligned on page boundaries, and in some cases we'd like to
     * have multiple references to the mapped area without creating additional
     * maps.
     *
     * This always uses MAP_SHARED.
     *
     * TODO: we should be able to create a new FileMap that is a subset of
     * an existing FileMap and shares the underlying mapped pages.  Requires
     * completing the refcounting stuff and possibly introducing the notion
     * of a FileMap hierarchy.
     */
    class FileMap {
    public:
        FileMap(void);

        FileMap(FileMap&& f) noexcept;
        FileMap& operator=(FileMap&& f) noexcept;

        /*
         * Create a new mapping on an open file.
         *
         * Closing the file descriptor does not unmap the pages, so we don't
         * claim ownership of the fd.
         *
         * Returns "false" on failure.
         */
        bool create(const char* origFileName, int fd,
            int64_t offset, size_t length, bool readOnly);
        
        ~FileMap(void);

        /*
         * Return the name of the file this map came from, if known.
         */
        const char* getFileName(void) const { return mFileName; }

        /*
         * Get a pointer to the piece of the file we requested.
         */
        void* getDataPtr(void) const { return mDataPtr; }

        /*
         * Get the length we requested.
         */
        size_t getDataLength(void) const { return mDataLength; }

        /*
         * Get the data offset used to create this map.
         */
        int64_t getDataOffset(void) const { return mDataOffset; }

        /*
         * This maps directly to madvise() values, but allows us to avoid
         * including <sys/mman.h> everywhere.
         */
        enum MapAdvice {
            NORMAL, RANDOM, SEQUENTIAL, WILLNEED, DONTNEED
        };

        /*
         * Apply an madvise() call to the entire file.
         *
         * Returns 0 on success, -1 on failure.
         */
        int advise(MapAdvice advice);

    protected:

    private:
        // these are not implemented
        FileMap(const FileMap& src);
        const FileMap& operator=(const FileMap& src);

        char* mFileName;      // original file name, if known
        void* mBasePtr;       // base of mmap area; page aligned
        size_t      mBaseLength;    // length, measured from "mBasePtr"
        int64_t     mDataOffset;    // offset used when map was created
        void* mDataPtr;       // start of requested data, offset from base
        size_t      mDataLength;    // length, measured from "mDataPtr"
#if defined(_WIN32)
        HANDLE      mFileHandle;    // Win32 file handle
        HANDLE      mFileMapping;   // Win32 file mapping handle
#endif

        static long mPageSize;
    };

}  // namespace android