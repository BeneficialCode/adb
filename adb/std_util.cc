#include "std_util.h"

namespace openscreen {

    std::string Join(const std::vector<std::string>& strings,
        const char* delimiter) {
        size_t size_to_reserve = 0;
        for (const auto& piece : strings) {
            size_to_reserve += piece.length();
        }
        std::string out;
        out.reserve(size_to_reserve);
        auto it = strings.begin();
        out += *it;
        for (++it; it != strings.end(); ++it) {
            out += delimiter + *it;
        }

        return out;
    }

}  // namespace openscreen