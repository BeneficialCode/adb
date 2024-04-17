#include "strings.h"

#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

namespace android {
    namespace base {

        #define CHECK_NE(a, b) \
            if ((a) == (b)) abort();

        std::vector<std::string> Split(const std::string& s,
            const std::string& delimiters) {
            CHECK_NE(delimiters.size(), 0U);
            std::vector<std::string> result;
            size_t base = 0;
            size_t found;
            while (true) {
                found = s.find_first_of(delimiters, base);
                result.push_back(s.substr(base, found - base));
                if (found == s.npos) break;
                base = found + 1;
            }
            return result;
        }

        std::string Trim(const std::string& s) {
            std::string result;
            if (s.size() == 0) {
                return result;
            }
            size_t start_index = 0;
            size_t end_index = s.size() - 1;
            // Skip initial whitespace.
            while (start_index < s.size()) {
                if (!isspace(s[start_index])) {
                    break;
                }
                start_index++;
            }
            // Skip terminating whitespace.
            while (end_index >= start_index) {
                if (!isspace(s[end_index])) {
                    break;
                }
                end_index--;
            }
            // All spaces, no beef.
            if (end_index < start_index) {
                return "";
            }
            // Start_index is the first non-space, end_index is the last one.
            return s.substr(start_index, end_index - start_index + 1);
        }

        // These cases are probably the norm, so we mark them extern in the header to
        // aid compile time and binary size.
        template<> std::string Join(const std::vector<std::string>& vec, char separator)
        {
            if (vec.empty()) {
                return "";
            }
            std::ostringstream result;
            result << *vec.begin();
            for (auto it = std::next(vec.begin()); it != vec.end(); ++it) {
                result << separator << *it;
            }
            return result.str();
        }

        template<> std::string Join(const std::vector<const char*>& vec, char separator)
        {
            if (vec.empty()) {
                return "";
            }
            std::ostringstream result;
            result << *vec.begin();
            for (auto it = std::next(vec.begin()); it != vec.end(); ++it) {
                result << separator << *it;
            }
            return result.str();
        }

        template<> std::string Join(const std::vector<std::string>& vec, const std::string& separator)
        {
            if (vec.empty()) {
                return "";
            }
            std::ostringstream result;
            result << *vec.begin();
            for (auto it = std::next(vec.begin()); it != vec.end(); ++it) {
                result << separator << *it;
            }
            return result.str();
        }

        template<> std::string Join(const std::vector<const char*>& vec, const std::string& separator)
        {
            if (vec.empty()) {
                return "";
            }
            std::ostringstream result;
            result << *vec.begin();
            for (auto it = std::next(vec.begin()); it != vec.end(); ++it) {
                result << separator << *it;
            }
            return result.str();
        }
        
        bool StartsWith(std::string_view s, std::string_view prefix) {
            return s.substr(0, prefix.size()) == prefix;
        }

        bool StartsWith(std::string_view s, char prefix) {
            return !s.empty() && s.front() == prefix;
        }

        bool StartsWithIgnoreCase(std::string_view s, std::string_view prefix) {
            return s.size() >= prefix.size() && strncmp(s.data(), prefix.data(), prefix.size()) == 0;
        }

        bool StartsWith(const std::string& s, const char* prefix) {
            return strncmp(s.c_str(), prefix, strlen(prefix)) == 0;
        }
        bool StartsWith(const std::string& s, const std::string& prefix) {
            return strncmp(s.c_str(), prefix.c_str(), prefix.size()) == 0;
        }
        bool StartsWithIgnoreCase(const std::string& s, const char* prefix) {
            return _stricmp(s.c_str(), prefix) == 0;
        }
        bool StartsWithIgnoreCase(const std::string& s, const std::string& prefix) {
            return _stricmp(s.c_str(), prefix.c_str()) == 0;
        }
        static bool EndsWith(const std::string& s, const char* suffix, size_t suffix_length,
            bool case_sensitive) {
            size_t string_length = s.size();
            if (suffix_length > string_length) {
                return false;
            }
            size_t offset = string_length - suffix_length;
            if (case_sensitive) {
				return strncmp(s.c_str() + offset, suffix, suffix_length) == 0;
			}
            return _stricmp(s.c_str() + offset, suffix) == 0;
        }
        bool EndsWith(const std::string& s, const char* suffix) {
            return EndsWith(s, suffix, strlen(suffix), true);
        }
        bool EndsWith(const std::string& s, const std::string& suffix) {
            return EndsWith(s, suffix.c_str(), suffix.size(), true);
        }
        bool EndsWithIgnoreCase(const std::string& s, const char* suffix) {
            return EndsWith(s, suffix, strlen(suffix), false);
        }
        bool EndsWithIgnoreCase(const std::string& s, const std::string& suffix) {
            return EndsWith(s, suffix.c_str(), suffix.size(), false);
        }
        bool EqualsIgnoreCase(const std::string& lhs, const std::string& rhs) {
            return _stricmp(lhs.c_str(), rhs.c_str()) == 0;
        }

        bool EndsWith(std::string_view s, std::string_view suffix) {
            return s.size() >= suffix.size() && s.substr(s.size() - suffix.size(), suffix.size()) == suffix;
        }

        bool EndsWith(std::string_view s, char suffix) {
            return !s.empty() && s.back() == suffix;
        }

        bool EndsWithIgnoreCase(std::string_view s, std::string_view suffix) {
            return s.size() >= suffix.size() &&
                _stricmp(s.data() + (s.size() - suffix.size()), suffix.data()) == 0;
        }
    }  // namespace base
}  // namespace android