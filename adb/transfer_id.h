#pragma once

#include <stddef.h>
#include <stdint.h>

#include <utility>

enum class TransferDirection : uint64_t {
    READ = 0,
    WRITE = 1,
};

struct TransferId {
    TransferDirection direction : 1;
    uint64_t id : 63;

    constexpr TransferId() : TransferId(TransferDirection::READ, 0) {}
    TransferId(const TransferId& copy) = default;
    TransferId(TransferId&& move) = default;

    TransferId& operator=(const TransferId& copy) = default;
    TransferId& operator=(TransferId&& move) = default;

private:
    constexpr TransferId(TransferDirection direction, uint64_t id) : direction(direction), id(id) {}

public:
    bool operator==(const TransferId& rhs) const {
        return static_cast<uint64_t>(*this) == static_cast<uint64_t>(rhs);
    }

    constexpr explicit operator uint64_t() const {
        return static_cast<uint64_t>(direction) << 63 | id;
    }

    static constexpr TransferId read(uint64_t id) {
        return TransferId(TransferDirection::READ, id);
    }

    static constexpr TransferId write(uint64_t id) {
        return TransferId(TransferDirection::WRITE, id);
    }

    static constexpr TransferId from_value(uint64_t value) {
        uint64_t mask = static_cast<uint64_t>(1) << 63;

        TransferId result;
        result.direction = static_cast<TransferDirection>(!!(value & mask));
        result.id = value & (mask - 1);
        return result;
    }
};

namespace std {
    template <>
    struct hash<TransferId> {
        size_t operator()(TransferId id) const { return hash<uint64_t>()(static_cast<uint64_t>(id)); }
    };
}  // namespace std