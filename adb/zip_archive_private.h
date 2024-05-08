#pragma once

#include "zip_archive.h"

#include <stdint.h>
#include <stdlib.h>

#include <memory>
#include <utility>
#include <vector>

#include "macros.h"
#include "mapped_file.h"
#include "memory.h"
#include "zip_cd_entry_map.h"
#include "zip_error.h"

class MappedZipFile {
public:
    explicit MappedZipFile(int fd, off64_t length = -1, off64_t offset = 0);

    explicit MappedZipFile(const void* address, size_t length)
        : base_ptr_(address), data_length_(static_cast<off64_t>(length)) {}

    int GetFileDescriptor() const;

    const void* GetBasePtr() const;

    off64_t GetFileOffset() const;

    off64_t GetFileLength() const;

    const uint8_t* ReadAtOffset(uint8_t* buf, size_t len, off64_t off) const;

private:
    std::unique_ptr<android::base::MappedFile> mapped_file_;

    const int fd_ = -1;
    const off64_t fd_offset_ = 0;

    const void* base_ptr_ = nullptr;
    mutable off64_t data_length_ = -1;
};

class CentralDirectory {
public:
    CentralDirectory(void) : base_ptr_(nullptr), length_(0) {}

    const uint8_t* GetBasePtr() const { return base_ptr_; }

    size_t GetMapLength() const { return length_; }

    void Initialize(const void* map_base_ptr, off64_t cd_start_offset, size_t cd_size);

private:
    const uint8_t* base_ptr_;
    size_t length_;
};

struct ZipArchive {
    // open Zip archive
    mutable MappedZipFile mapped_zip;
    const bool close_file;

    // mapped central directory area
    off64_t directory_offset;
    CentralDirectory central_directory;
    std::unique_ptr<android::base::MappedFile> directory_map;

    // number of entries in the Zip archive
    uint64_t num_entries;
    std::unique_ptr<CdEntryMapInterface> cd_entry_map;

    ZipArchive(MappedZipFile&& map, bool assume_ownership);
    ZipArchive(const void* address, size_t length);
    ~ZipArchive();

    bool InitializeCentralDirectory(off64_t cd_start_offset, size_t cd_size);
};

// Reads the unaligned data of type |T| and auto increment the offset.
template <typename T>
static T ConsumeUnaligned(uint8_t** address) {
    auto ret = android::base::get_unaligned<T>(*address);
    *address += sizeof(T);
    return ret;
}

template <typename T>
static T ConsumeUnaligned(const uint8_t** address) {
    return ConsumeUnaligned<T>(const_cast<uint8_t**>(address));
}

// Writes the unaligned data of type |T| and auto increment the offset.
template <typename T>
void EmitUnaligned(uint8_t** address, T data) {
    android::base::put_unaligned<T>(*address, data);
    *address += sizeof(T);
}