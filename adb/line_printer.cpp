#include "line_printer.h"

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
#else

#endif

#include "sysdeps.h"

#include <vector>

using namespace std;
// This does not account for multiple UTF-8 bytes corresponding to a single Unicode code point, or
// multiple code points corresponding to a single grapheme cluster (user-perceived character).
string ElideMiddle(const string& str, size_t width) {
    const int kMargin = 3;  // Space for "...".
    string result = str;
    if (result.size() + kMargin > width) {
        size_t elide_size = (width - kMargin) / 2;
        result = result.substr(0, elide_size)
            + "..."
            + result.substr(result.size() - elide_size, elide_size);
    }
    return result;
}

LinePrinter::LinePrinter() : have_blank_line_(true) {
#ifndef _WIN32
    const char* term = getenv("TERM");
    smart_terminal_ = unix_isatty(1) && term && string(term) != "dumb";
#else
    // Disable output buffer.  It'd be nice to use line buffering but
    // MSDN says: "For some systems, [_IOLBF] provides line
    // buffering. However, for Win32, the behavior is the same as _IOFBF
    // - Full Buffering."
    setvbuf(stdout, nullptr, _IONBF, 0);
    console_ = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    smart_terminal_ = GetConsoleScreenBufferInfo(console_, &csbi);
#endif
}

static void Out(const std::string& s) {
    // Avoid printf and C strings, since the actual output might contain null
    // bytes like UTF-16 does (yuck).
    fwrite(s.data(), 1, s.size(), stdout);
}

void LinePrinter::Print(string to_print, LineType type) {
    if (!smart_terminal_) {
        if (type == LineType::INFO) {
            info_line_ = to_print + "\n";
        }
        else {
            Out(to_print + "\n");
        }
        return;
    }

    // Print over previous line, if any.
    // On Windows, calling a C library function writing to stdout also handles
    // pausing the executable when the "Pause" key or Ctrl-S is pressed.
    printf("\r");

    if (type == INFO) {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(console_, &csbi);

        to_print = ElideMiddle(to_print, static_cast<size_t>(csbi.dwSize.X));
        std::wstring to_print_wide;
        // ElideMiddle may create invalid UTF-8, so ignore conversion errors.
        (void)android::base::UTF8ToWide(to_print, &to_print_wide);
        // We don't want to have the cursor spamming back and forth, so instead of
        // printf use WriteConsoleOutput which updates the contents of the buffer,
        // but doesn't move the cursor position.
        COORD buf_size = { csbi.dwSize.X, 1 };
        COORD zero_zero = { 0, 0 };
        SMALL_RECT target = {
          csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y,
          static_cast<SHORT>(csbi.dwCursorPosition.X + csbi.dwSize.X - 1),
          csbi.dwCursorPosition.Y
        };
        vector<CHAR_INFO> char_data(csbi.dwSize.X);
        for (size_t i = 0; i < static_cast<size_t>(csbi.dwSize.X); ++i) {
            char_data[i].Char.UnicodeChar = i < to_print_wide.size() ? to_print_wide[i] : L' ';
            char_data[i].Attributes = csbi.wAttributes;
        }
        WriteConsoleOutputW(console_, &char_data[0], buf_size, zero_zero, &target);
#else
        
#endif

        have_blank_line_ = false;
    }
    else {
        Out(to_print);
        Out("\n");
        have_blank_line_ = true;
    }
}

void LinePrinter::KeepInfoLine() {
    if (smart_terminal_) {
        if (!have_blank_line_) Out("\n");
        have_blank_line_ = true;
    }
    else {
        Out(info_line_);
        info_line_.clear();
    }
}