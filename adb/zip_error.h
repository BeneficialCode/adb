#pragma once

#include <stdint.h>

enum ZipError : int32_t {
	kSuccess = 0,

	kIterationEnd = -1,

	// We encountered a Zlib error when inflating a stream from this file.
	// Usually indicates file corruption.
	kZlibError = -2,

	// The input file cannot be processed as a zip archive. Usually because
	// it's too small, too large or does not have a valid signature.
	kInvalidFile = -3,

	// An invalid iteration / ziparchive handle was passed in as an input
	// argument.
	kInvalidHandle = -4,

	// The zip archive contained two (or possibly more) entries with the same
	// name.
	kDuplicateEntry = -5,

	// The zip archive contains no entries.
	kEmptyArchive = -6,

	// The specified entry was not found in the archive.
	kEntryNotFound = -7,

	// The zip archive contained an invalid local file header pointer.
	kInvalidOffset = -8,

	// The zip archive contained inconsistent entry information. This could
	// be because the central directory & local file header did not agree, or
	// if the actual uncompressed length or crc32 do not match their declared
	// values.
	kInconsistentInformation = -9,

	// An invalid entry name was encountered.
	kInvalidEntryName = -10,

	// An I/O related system call (read, lseek, ftruncate, map) failed.
	kIoError = -11,

	// We were not able to mmap the central directory or entry contents.
	kMmapFailed = -12,

	// An allocation failed.
	kAllocationFailed = -13,

	// The compressed or uncompressed size is larger than UINT32_MAX and
	// doesn't fit into the 32 bits zip entry.
	kUnsupportedEntrySize = -14,

	kLastErrorCode = kUnsupportedEntrySize,
};