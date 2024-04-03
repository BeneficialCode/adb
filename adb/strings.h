#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace android {
    namespace base {
        // Splits a string into a vector of strings.
        //
        // The string is split at each occurrence of a character in delimiters.
        //
        // The empty string is not a valid delimiter list.
        std::vector<std::string> Split(const std::string& s,
            const std::string& delimiters);

        // Trims whitespace off both ends of the given string.
        std::string Trim(const std::string& s);
        // Joins a container of things into a single string, using the given separator.
        template <typename ContainerT, typename SeparatorT>
        std::string Join(const ContainerT& things, SeparatorT separator) {
            if (things.empty()) {
                return "";
            }
            std::ostringstream result;
            result << *things.begin();
            for (auto it = std::next(things.begin()); it != things.end(); ++it) {
                result << separator << *it;
            }
            return result.str();
        }
        // We instantiate the common cases in strings.cpp.
        template<> std::string Join(const std::vector<std::string>&, char);
        template<> std::string Join(const std::vector<const char*>&, char);
        template<> std::string Join(const std::vector<std::string>&, const std::string&);
        template<> std::string Join(const std::vector<const char*>&, const std::string&);

        // Tests whether 's' starts with 'prefix'.
        bool StartsWith(std::string_view s, std::string_view prefix);
        bool StartsWith(std::string_view s, char prefix);
        bool StartsWithIgnoreCase(std::string_view s, std::string_view prefix);

        bool StartsWith(const std::string& s, const char* prefix);
        bool StartsWithIgnoreCase(const std::string& s, const char* prefix);
        bool StartsWith(const std::string& s, const std::string& prefix);
        bool StartsWithIgnoreCase(const std::string& s, const std::string& prefix);
        // Tests whether 's' ends with 'suffix'.
        // TODO: string_view
        bool EndsWith(const std::string& s, const char* suffix);
        bool EndsWithIgnoreCase(const std::string& s, const char* suffix);
        bool EndsWith(const std::string& s, const std::string& suffix);
        bool EndsWithIgnoreCase(const std::string& s, const std::string& suffix);

        // Removes `prefix` from the start of the given string and returns true (if
        // it was present), false otherwise.
        inline bool ConsumePrefix(std::string_view* s, std::string_view prefix) {
            if (!StartsWith(*s, prefix)) return false;
            s->remove_prefix(prefix.size());
            return true;
        }
    }  // namespace base
}  // namespace android