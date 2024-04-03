#pragma once
/* A cross-platform equivalent of bionic's <sys/endian.h>. */
/* For __BIONIC__ and __GLIBC__ */

#if defined(__BIONIC__)
#include <sys/endian.h>
#elif defined(__GLIBC__)

#else
#if defined(__APPLE__)
/* macOS has some of the basics. */
#include <sys/_endian.h>
#else
#include <Windows.h>
/* Windows has some of the basics as well. */
#include <winsock2.h>
/* winsock2.h *must* be included before the following four macros. */
#define htons(x) __builtin_bswap16(x)
#define htonl(x) __builtin_bswap32(x)
#define ntohs(x) __builtin_bswap16(x)
#define ntohl(x) __builtin_bswap32(x)
#endif
/* Neither macOS nor Windows have the rest. */
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __BYTE_ORDER __LITTLE_ENDIAN
#define htonq(x) __builtin_bswap64(x)
#define ntohq(x) __builtin_bswap64(x)
#define htobe16(x) __builtin_bswap16(x)
#define htobe32(x) __builtin_bswap32(x)
#define htobe64(x) __builtin_bswap64(x)
#define betoh16(x) __builtin_bswap16(x)
#define betoh32(x) __builtin_bswap32(x)
#define betoh64(x) __builtin_bswap64(x)
#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define letoh16(x) (x)
#define letoh32(x) (x)
#define letoh64(x) (x)
#define be16toh(x) __builtin_bswap16(x)
#define be32toh(x) __builtin_bswap32(x)
#define be64toh(x) __builtin_bswap64(x)
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)
#endif