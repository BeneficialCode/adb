#include "shell_protocol.h"

#include <string.h>

#include <algorithm>

#include "adb_io.h"

ShellProtocol::ShellProtocol(borrowed_fd fd) : fd_(fd) {
    buffer_[0] = kIdInvalid;
}

ShellProtocol::~ShellProtocol() {
}

bool ShellProtocol::Read() {
    // Only read a new header if we've finished the last packet.
    if (!bytes_left_) {
        if (!ReadFdExactly(fd_, buffer_, kHeaderSize)) {
            return false;
        }

        length_t packet_length;
        memcpy(&packet_length, &buffer_[1], sizeof(packet_length));
        bytes_left_ = packet_length;
        data_length_ = 0;
    }

    size_t read_length = std::min(bytes_left_, data_capacity());
    if (read_length && !ReadFdExactly(fd_, data(), read_length)) {
        return false;
    }

    bytes_left_ -= read_length;
    data_length_ = read_length;

    return true;
}

bool ShellProtocol::Write(Id id, size_t length) {
    buffer_[0] = id;
    length_t typed_length = length;
    memcpy(&buffer_[1], &typed_length, sizeof(typed_length));

    return WriteFdExactly(fd_, buffer_, kHeaderSize + length);
}

