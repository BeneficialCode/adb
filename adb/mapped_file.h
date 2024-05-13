#pragma once

#include <sys/types.h>
#include <memory>
#include "macros.h"
#include "off64_t.h"
#include "unique_fd.h"

#if defined(_WIN32)
#define PROT_READ 1
#define PROT_WRITE 2
#include <Windows.h>
using os_handle = HANDLE;
#else
#include <sys/mman.h>
using os_handle = int;
#endif
namespace android {
    namespace base {
        /**
         * A region of a file mapped into memory (for grepping: also known as MmapFile or file mapping).
         */
        class MappedFile {
        public:
            /**
             * Creates a new mapping of the file pointed to by `fd`. Unlike the underlying OS primitives,
             * `offset` does not need to be page-aligned. If `PROT_WRITE` is set in `prot`, the mapping
             * will be writable, otherwise it will be read-only. Mappings are always `MAP_SHARED`.
             */
            static std::unique_ptr<MappedFile> FromFd(borrowed_fd fd, off64_t offset, size_t length,
                int prot);
            /**
             * Same thing, but using the raw OS file handle instead of a CRT wrapper.
             */
            static std::unique_ptr<MappedFile> FromOsHandle(os_handle h, off64_t offset, size_t length,
                int prot);
            /**
             * Removes the mapping.
             */
            ~MappedFile();
            /**
             * Not copyable but movable.
             */
            MappedFile(MappedFile&& other);
            MappedFile& operator=(MappedFile&& other);
            char* data() const { return base_ + offset_; }
            size_t size() const { return size_; }
        private:
            DISALLOW_IMPLICIT_CONSTRUCTORS(MappedFile);
            void Close();
            char* base_;
            size_t size_;
            size_t offset_;
#if defined(_WIN32)
            MappedFile(char* base, size_t size, size_t offset, HANDLE handle)
                : base_(base), size_(size), offset_(offset), handle_(handle) {}
            HANDLE handle_;
#else
            MappedFile(char* base, size_t size, size_t offset) : base_(base), size_(size), offset_(offset) {}
#endif
        };
    }  // namespace base
}  // namespace android