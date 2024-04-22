#pragma once

#include <vector>

#include "adb.h"
#include "commandline.h"
#include "line_printer.h"

class Bugreport {
    friend class BugreportStandardStreamsCallback;

public:
    Bugreport() : line_printer_() {
    }
    int DoIt(int argc, const char** argv);

protected:
    // Functions below are abstractions of external functions so they can be
    // mocked on tests.
    virtual int SendShellCommand(
        const std::string& command, bool disable_shell_protocol,
        StandardStreamsCallbackInterface* callback = &DEFAULT_STANDARD_STREAMS_CALLBACK);

    virtual bool DoSyncPull(const std::vector<const char*>& srcs, const char* dst, bool copy_attrs,
        const char* name);

private:
    virtual void UpdateProgress(const std::string& file_name, int progress_percentage);
    LinePrinter line_printer_;
    DISALLOW_COPY_AND_ASSIGN(Bugreport);
};