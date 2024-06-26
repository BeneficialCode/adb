#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "stringprintf.h"

namespace openscreen {

    template <typename T, size_t N>
    constexpr size_t countof(T(&array)[N]) {
        return N;
    }

    // std::basic_string::data() has no mutable overload prior to C++17 [1].
    // Hence this overload is provided.
    // Note: str[0] is safe even for empty strings, as they are guaranteed to be
    // null-terminated [2].
    //
    // [1] http://en.cppreference.com/w/cpp/string/basic_string/data
    // [2] http://en.cppreference.com/w/cpp/string/basic_string/operator_at
    template <typename CharT, typename Traits, typename Allocator>
    CharT* data(std::basic_string<CharT, Traits, Allocator>& str) {
        return std::addressof(str[0]);
    }

    std::string Join(const std::vector<std::string>& strings,
        const char* delimiter);

    template <typename Key, typename Value>
    void RemoveValueFromMap(std::map<Key, Value*>* map, Value* value) {
        for (auto it = map->begin(); it != map->end();) {
            if (it->second == value) {
                it = map->erase(it);
            }
            else {
                ++it;
            }
        }
    }

    template <typename RandomAccessContainer>
    void SortAndDedupeElements(RandomAccessContainer* c) {
        std::sort(c->begin(), c->end());
        const auto new_end = std::unique(c->begin(), c->end());
        c->erase(new_end, c->end());
    }

    // Append the provided elements together into a single vector. This can be
    // useful when creating a vector of variadic templates in the ctor.
    //
    // This is the base case for the recursion
    template <typename T>
    std::vector<T>&& Append(std::vector<T>&& so_far) {
        return std::move(so_far);
    }

    // This is the recursive call. Depending on the number of remaining elements, it
    // either calls into itself or into the above base case.
    template <typename T, typename TFirst, typename... TOthers>
    std::vector<T>&& Append(std::vector<T>&& so_far,
        TFirst&& new_element,
        TOthers&&... new_elements) {
        so_far.push_back(std::move(new_element));
        return Append(std::move(so_far), std::move(new_elements)...);
    }

    // Creates an empty vector with |size| elements reserved. Intended to be used as
    // GetEmptyVectorOfSize<T>(sizeof...(variadic_input))
    template <typename T>
    std::vector<T> GetVectorWithCapacity(size_t size) {
        std::vector<T> results;
        results.reserve(size);
        return results;
    }

}  // namespace openscreen