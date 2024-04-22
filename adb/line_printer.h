#pragma once

#include <stddef.h>
#include <string>

/// Prints lines of text, possibly overprinting previously printed lines
/// if the terminal supports it.
struct LinePrinter {
	LinePrinter();

	bool is_smart_terminal() const { return smart_terminal_; }
	void set_smart_terminal(bool smart) { smart_terminal_ = smart; }

	enum LineType { INFO, WARNING, ERROR };

	/// Outputs the given line. INFO output will be overwritten.
	/// WARNING and ERROR appear on a line to themselves.
	void Print(std::string to_print, LineType type);

	/// If there's an INFO line, keep it. If not, do nothing.
	void KeepInfoLine();

private:
	/// Whether we can do fancy terminal control codes.
	bool smart_terminal_;

	/// Whether the caret is at the beginning of a blank line.
	bool have_blank_line_;

	/// The last printed info line when printing to a dumb terminal.
	std::string info_line_;

#ifdef _WIN32
	void* console_;
#endif
};