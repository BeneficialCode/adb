#pragma once

#include "adb_unique_fd.h"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <stdint.h>

#include "off64_t.h"

namespace incremental {

    using Size = int64_t;
    constexpr int kBlockSize = 4096;
    constexpr int kSha256DigestSize = 32;
    constexpr int kDigestSize = kSha256DigestSize;
    constexpr int kMaxSignatureSize = 8096;  // incrementalfs.h

    constexpr std::string_view IDSIG = ".idsig";

    std::vector<int32_t> PriorityBlocksForFile(const std::string& filepath, borrowed_fd fd,
        Size fileSize);

    Size verity_tree_blocks_for_file(Size fileSize);
    Size verity_tree_size_for_file(Size fileSize);

    std::pair<std::vector<char>, int32_t> read_id_sig_headers(borrowed_fd fd);
    std::pair<off64_t, size_t> skip_id_sig_headers(borrowed_fd fd);

}  // namespace incremental