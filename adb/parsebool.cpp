#include "parsebool.h"
#include <errno.h>

namespace android {
    namespace base {

        ParseBoolResult ParseBool(std::string_view s) {
            if (s == "1" || s == "y" || s == "yes" || s == "on" || s == "true") {
                return ParseBoolResult::kTrue;
            }
            if (s == "0" || s == "n" || s == "no" || s == "off" || s == "false") {
                return ParseBoolResult::kFalse;
            }
            return ParseBoolResult::kError;
        }

    }  // namespace base
}  // namespace android