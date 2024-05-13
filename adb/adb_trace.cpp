#include "sysdeps.h"
#include "adb_trace.h"

#include <string>
#include <unordered_map>
#include <vector>

#include "logging.h"
#include "strings.h"

#include "adb.h"

const char* adb_device_banner = "host";

void AdbLogger(android::base::LogId id,android::base::LogSeverity severity,
	const char* tag, const char* file, unsigned int line,
	const char* message) {
	android::base::StderrLogger(id,severity, tag, file, line, message);
	fflush(stderr);
}

int adb_trace_mask;

std::string get_trace_setting() {
	const char* setting = getenv("ADB_TRACE");
    if (setting == nullptr) {
        setting = "";
    }
    return setting;
}

// Split the space separated list of tags from the trace setting and build the
// trace mask from it. note that '1' and 'all' are special cases to enable all
// tracing.
//
// adb's trace setting comes from the ADB_TRACE environment variable, whereas
// adbd's comes from the system property persist.adb.trace_mask.
static void setup_trace_mask() {
    const std::string trace_setting = get_trace_setting();
    if (trace_setting.empty()) {
        return;
    }

    std::unordered_map<std::string, int> trace_flags = { {"1", -1},
                                                        {"all", -1},
                                                        {"adb", ADB},
                                                        {"sockets", SOCKETS},
                                                        {"packets", PACKETS},
                                                        {"rwx", RWX},
                                                        {"usb", USB},
                                                        {"sync", SYNC},
                                                        {"sysdeps", SYSDEPS},
                                                        {"transport", TRANSPORT},
                                                        {"jdwp", JDWP},
                                                        {"services", SERVICES},
                                                        {"auth", AUTH},
                                                        {"fdevent", FDEVENT},
                                                        {"shell", SHELL},
                                                        {"incremental", INCREMENTAL} };

    std::vector<std::string> elements = android::base::Split(trace_setting, " ");
    for (const auto& elem : elements) {
        const auto& flag = trace_flags.find(elem);
        if (flag == trace_flags.end()) {
            LOG(ERROR) << "Unknown trace flag: " << elem;
            continue;
        }

        if (flag->second == -1) {
            // -1 is used for the special values "1" and "all" that enable all
            // tracing.
            adb_trace_mask = ~0;
            break;
        }
        else {
            adb_trace_mask |= 1 << flag->second;
        }
    }

    if (adb_trace_mask != 0) {
        android::base::SetMinimumLogSeverity(android::base::VERBOSE);
    }
}

void adb_trace_init(char** argv){
	android::base::InitLogging(argv, &AdbLogger);

    setup_trace_mask();

    VLOG(ADB) << adb_version();
}

void adb_trace_enable(AdbTrace trace_tag) {
    adb_trace_mask |= (1 << trace_tag);
}
