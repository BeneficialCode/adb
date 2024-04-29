#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "Unicode.h"

namespace android {

    template <class T>
    using BasicStringPiece = std::basic_string_view<T>;

    using StringPiece = BasicStringPiece<char>;
    using StringPiece16 = BasicStringPiece<char16_t>;

}  // namespace android

namespace std {

    inline ::std::ostream& operator<<(::std::ostream& out, ::std::u16string_view str) {
        size_t utf8_len = utf16_to_utf8_length(str.data(), str.size());
        if (utf8_len < 0) {
            return out;  // empty
        }

        std::string utf8;
        utf8.resize(static_cast<size_t>(utf8_len));
        utf16_to_utf8(str.data(), str.size(), utf8.data(), utf8_len + 1);
        return out << utf8;
    }

    inline ::std::ostream& operator<<(::std::ostream& out, const ::std::u16string& str) {
        return out << std::u16string_view(str);
    }

}  // namespace std
