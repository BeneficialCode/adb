#pragma once
#include <sys/stat.h>
#include <sys/types.h>

// stat is broken on Win32: stat on a path with a trailing slash or backslash will always fail with
// ENOENT.
int adb_stat(const char* path, struct adb_stat* buf);

// We later define a macro mapping 'stat' to 'adb_stat'. This causes:
//   struct stat s;
//   stat(filename, &s);
// To turn into the following:
//   struct adb_stat s;
//   adb_stat(filename, &s);
// To get this to work, we need to make 'struct adb_stat' the same as
// 'struct stat'. Note that this definition of 'struct adb_stat' uses the
// *current* macro definition of stat, so it may actually be inheriting from
// struct _stat32i64 (or some other remapping).
struct adb_stat : public stat {};

#undef stat
#define stat adb_stat

// Windows doesn't have lstat.
#define lstat adb_stat

// mingw doesn't define S_IFLNK or S_ISLNK.
#define S_IFLNK 0120000
#define S_ISLNK(mode) (((mode) & S_IFMT) == S_IFLNK)

#define S_IFBLK 0060000
#define S_ISBLK(mode) (((mode) & S_IFMT) == S_IFBLK)

// Make sure that host file mode values match the ones on the device.
static_assert(S_IFMT == 00170000, "");
static_assert(S_IFLNK == 0120000, "");
static_assert(S_IFREG == 0100000, "");
static_assert(S_IFBLK == 0060000, "");
static_assert(S_IFDIR == 0040000, "");
static_assert(S_IFCHR == 0020000, "");