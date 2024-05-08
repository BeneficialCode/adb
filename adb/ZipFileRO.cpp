#define LOG_TAG "zipro"
//#define LOG_NDEBUG 0
#include "ZipFileRO.h"
#include "Compat.h"
#include "misc.h"
#include "threads.h"
#include "zip_archive.h"

#include <zlib.h>

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>


using namespace android;

class _ZipEntryRO {
public:
    ZipEntry entry;
    std::string_view name;
    void* cookie = nullptr;

    _ZipEntryRO() = default;

    ~_ZipEntryRO() {
        EndIteration(cookie);
    }

    android::ZipEntryRO convertToPtr() {
        _ZipEntryRO* result = new _ZipEntryRO;
        result->entry = std::move(this->entry);
        result->name = std::move(this->name);
        result->cookie = std::exchange(this->cookie, nullptr);
        return result;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(_ZipEntryRO);
};

ZipFileRO::~ZipFileRO() {
    CloseArchive(mHandle);
    if (mFileName != NULL) {
        free(mFileName);
    }
}

/*
 * Open the specified file read-only.  We memory-map the entire thing and
 * close the file before returning.
 */
/* static */ ZipFileRO* ZipFileRO::open(const char* zipFileName)
{
    ZipArchiveHandle handle;
    const int32_t error = OpenArchive(zipFileName, &handle);
    if (error) {
       
        CloseArchive(handle);
        return NULL;
    }

    return new ZipFileRO(handle, _strdup(zipFileName));
}


/* static */ ZipFileRO* ZipFileRO::openFd(int fd, const char* debugFileName,
    bool assume_ownership)
{
    ZipArchiveHandle handle;
    const int32_t error = OpenArchiveFd(fd, debugFileName, &handle, assume_ownership);
    if (error) {
        CloseArchive(handle);
        return NULL;
    }

    return new ZipFileRO(handle, _strdup(debugFileName));
}

ZipEntryRO ZipFileRO::findEntryByName(const char* entryName) const
{
    _ZipEntryRO data;
    data.name = entryName;

    const int32_t error = FindEntry(mHandle, entryName, &(data.entry));
    if (error) {
        return nullptr;
    }

    return data.convertToPtr();
}

/*
 * Get the useful fields from the zip entry.
 *
 * Returns "false" if the offsets to the fields or the contents of the fields
 * appear to be bogus.
 */
bool ZipFileRO::getEntryInfo(ZipEntryRO entry, uint16_t* pMethod,
    uint32_t* pUncompLen, uint32_t* pCompLen, off64_t* pOffset,
    uint32_t* pModWhen, uint32_t* pCrc32) const
{
    return getEntryInfo(entry, pMethod, pUncompLen, pCompLen, pOffset, pModWhen,
        pCrc32, nullptr);
}

bool ZipFileRO::getEntryInfo(ZipEntryRO entry, uint16_t* pMethod,
    uint32_t* pUncompLen, uint32_t* pCompLen, off64_t* pOffset,
    uint32_t* pModWhen, uint32_t* pCrc32, uint16_t* pExtraFieldSize) const
{
    const _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const ZipEntry& ze = zipEntry->entry;

    if (pMethod != nullptr) {
        *pMethod = ze.method;
    }
    if (pUncompLen != nullptr) {
        *pUncompLen = ze.uncompressed_length;
    }
    if (pCompLen != nullptr) {
        *pCompLen = ze.compressed_length;
    }
    if (pOffset != nullptr) {
        *pOffset = ze.offset;
    }
    if (pModWhen != nullptr) {
        *pModWhen = ze.mod_time;
    }
    if (pCrc32 != nullptr) {
        *pCrc32 = ze.crc32;
    }
    if (pExtraFieldSize != nullptr) {
        *pExtraFieldSize = ze.extra_field_size;
    }

    return true;
}

bool ZipFileRO::startIteration(void** cookie) {
    return startIteration(cookie, nullptr, nullptr);
}

bool ZipFileRO::startIteration(void** cookie, const char* prefix, const char* suffix) {
    auto result = startIterationOrError(prefix, suffix);
    if (!result.ok()) {
        return false;
    }
    *cookie = result.value();
    return true;
}

base::expected<void*, int32_t>
ZipFileRO::startIterationOrError(const char* prefix, const char* suffix) {
    _ZipEntryRO ze;
    int32_t error = StartIteration(mHandle, &(ze.cookie),
        prefix ? prefix : "", suffix ? suffix : "");
    if (error) {
        
        return base::unexpected(error);
    }

    return ze.convertToPtr();
}

ZipEntryRO ZipFileRO::nextEntry(void* cookie) {
    auto result = nextEntryOrError(cookie);
    if (!result.ok()) {
        return nullptr;
    }
    return result.value();
}

base::expected<ZipEntryRO, int32_t> ZipFileRO::nextEntryOrError(void* cookie) {
    _ZipEntryRO* ze = reinterpret_cast<_ZipEntryRO*>(cookie);
    int32_t error = Next(ze->cookie, &(ze->entry), &(ze->name));
    if (error) {
        if (error != -1) {
            
            return base::unexpected(error);
        }
        return nullptr;
    }

    return &(ze->entry);
}

void ZipFileRO::endIteration(void* cookie)
{
    delete reinterpret_cast<_ZipEntryRO*>(cookie);
}

void ZipFileRO::releaseEntry(ZipEntryRO entry) const
{
    delete reinterpret_cast<_ZipEntryRO*>(entry);
}

/*
 * Copy the entry's filename to the buffer.
 */
int ZipFileRO::getEntryFileName(ZipEntryRO entry, char* buffer, size_t bufLen)
const
{
    const _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const uint16_t requiredSize = zipEntry->name.length() + 1;

    if (bufLen < requiredSize) {
        
        return requiredSize;
    }

    memcpy(buffer, zipEntry->name.data(), requiredSize - 1);
    buffer[requiredSize - 1] = '\0';

    return 0;
}

/*
 * Create a new FileMap object that spans the data in "entry".
 */
FileMap* ZipFileRO::createEntryFileMap(ZipEntryRO entry) const
{
    const _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const ZipEntry& ze = zipEntry->entry;
    int fd = GetFileDescriptor(mHandle);
    size_t actualLen = 0;

    if (ze.method == kCompressStored) {
        actualLen = ze.uncompressed_length;
    }
    else {
        actualLen = ze.compressed_length;
    }

    FileMap* newMap = new FileMap();
    if (!newMap->create(mFileName, fd, ze.offset, actualLen, true)) {
        delete newMap;
        return NULL;
    }

    return newMap;
}

/*
 * Create a new incfs::IncFsFileMap object that spans the data in "entry".
 */
std::optional<incfs::IncFsFileMap> ZipFileRO::createEntryIncFsFileMap(ZipEntryRO entry) const
{
    const _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const ZipEntry& ze = zipEntry->entry;
    int fd = GetFileDescriptor(mHandle);
    size_t actualLen = 0;

    if (ze.method == kCompressStored) {
        actualLen = ze.uncompressed_length;
    }
    else {
        actualLen = ze.compressed_length;
    }

    incfs::IncFsFileMap newMap;
    if (!newMap.Create(fd, ze.offset, actualLen, mFileName)) {
        return std::nullopt;
    }
    return std::move(newMap);
}

/*
 * Uncompress an entry, in its entirety, into the provided output buffer.
 *
 * This doesn't verify the data's CRC, which might be useful for
 * uncompressed data.  The caller should be able to manage it.
 */
bool ZipFileRO::uncompressEntry(ZipEntryRO entry, void* buffer, size_t size) const
{
    _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const int32_t error = ExtractToMemory(mHandle, &(zipEntry->entry),
        (uint8_t*)buffer, size);
    if (error) {
       
        return false;
    }

    return true;
}

/*
 * Uncompress an entry, in its entirety, to an open file descriptor.
 *
 * This doesn't verify the data's CRC, but probably should.
 */
bool ZipFileRO::uncompressEntry(ZipEntryRO entry, int fd) const
{
    _ZipEntryRO* zipEntry = reinterpret_cast<_ZipEntryRO*>(entry);
    const int32_t error = ExtractEntryToFile(mHandle, &(zipEntry->entry), fd);
    if (error) {
       
        return false;
    }

    return true;
}

const char* ZipFileRO::getZipFileName() {
    return mFileName;
}
