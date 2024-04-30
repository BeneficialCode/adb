#include "patch_utils.h"

#include <stdio.h>

#include "adb_io.h"
#include "adb_utils.h"
#include "endian.h"
#include "sysdeps.h"

#include "apk_archive.h"

using namespace com::android;
using namespace com::android::fastdeploy;
using namespace android::base;

static constexpr char kSignature[] = "FASTDEPLOY";

APKMetaData PatchUtils::GetDeviceAPKMetaData(const APKDump& apk_dump) {
    APKMetaData apkMetaData;
    apkMetaData.set_absolute_path(apk_dump.absolute_path());

    std::string md5Hash;
    int64_t localFileHeaderOffset;
    int64_t dataSize;

    const auto& cd = apk_dump.cd();
    auto cur = cd.data();
    int64_t size = cd.size();
    while (auto consumed = ApkArchive::ParseCentralDirectoryRecord(
        cur, size, &md5Hash, &localFileHeaderOffset, &dataSize)) {
        cur += consumed;
        size -= consumed;

        auto apkEntry = apkMetaData.add_entries();
        apkEntry->set_md5(md5Hash);
        apkEntry->set_dataoffset(localFileHeaderOffset);
        apkEntry->set_datasize(dataSize);
    }
    return apkMetaData;
}

APKMetaData PatchUtils::GetHostAPKMetaData(const char* apkPath) {
    ApkArchive archive(apkPath);
    auto dump = archive.ExtractMetadata();
    if (dump.cd().empty()) {
        fprintf(stderr, "adb: Could not extract Central Directory from %s\n", apkPath);
        error_exit("Aborting");
    }

    auto apkMetaData = GetDeviceAPKMetaData(dump);

    // Now let's set data sizes.
    for (auto& apkEntry : *apkMetaData.mutable_entries()) {
        auto dataSize =
            archive.CalculateLocalFileEntrySize(apkEntry.dataoffset(), apkEntry.datasize());
        if (dataSize == 0) {
            error_exit("Aborting");
        }
        apkEntry.set_datasize(dataSize);
    }

    return apkMetaData;
}

void PatchUtils::WriteSignature(borrowed_fd output) {
    WriteFdExactly(output, kSignature, sizeof(kSignature) - 1);
}

void PatchUtils::WriteLong(int64_t value, borrowed_fd output) {
    int64_t littleEndian = htole64(value);
    WriteFdExactly(output, &littleEndian, sizeof(littleEndian));
}

void PatchUtils::WriteString(const std::string& value, android::base::borrowed_fd output) {
    WriteLong(value.size(), output);
    WriteFdExactly(output, value);
}

void PatchUtils::Pipe(borrowed_fd input, borrowed_fd output, size_t amount) {
    constexpr static size_t BUFFER_SIZE = 128 * 1024;
    char buffer[BUFFER_SIZE];
    size_t transferAmount = 0;
    while (transferAmount != amount) {
        auto chunkAmount = std::min(amount - transferAmount, BUFFER_SIZE);
        auto readAmount = adb_read(input, buffer, chunkAmount);
        if (readAmount < 0) {
            error_exit("Aborting");
        }
        WriteFdExactly(output, buffer, readAmount);
        transferAmount += readAmount;
    }
}
