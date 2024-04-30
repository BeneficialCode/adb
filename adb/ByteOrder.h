#pragma once


/*
 * If you're looking for a portable <endian.h> that's available on Android,
 * Linux, macOS, and Windows, see <android-base/endian.h> instead.
 *
 * Nothing in this file is useful because all supported Android ABIs are
 * little-endian and all our code that runs on the host assumes that the host is
 * also little-endian. What pretense at big-endian support exists is completely
 * untested and unlikely to actually work.
 */

#include <stdint.h>
#include <sys/types.h>
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif


 /* TODO: move this cruft to frameworks/. */

#define dtohl(x) (x)
#define dtohs(x) (x)
#define htodl(x) (x)
#define htods(x) (x)

#define fromlel(x) (x)
#define tolel(x) (x)