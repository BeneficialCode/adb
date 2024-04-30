#pragma once



#if !defined(__MINGW32__)

#endif

#if defined(__APPLE__)

/* Mac OS has always had a 64-bit off_t, so it doesn't have off64_t. */
static_assert(sizeof(off_t) >= 8, "This code requires that Mac OS have at least a 64-bit off_t.");
typedef off_t off64_t;

static inline void* mmap64(void* addr, size_t length, int prot, int flags, int fd, off64_t offset) {
    return mmap(addr, length, prot, flags, fd, offset);
}

static inline off64_t lseek64(int fd, off64_t offset, int whence) {
    return lseek(fd, offset, whence);
}

static inline ssize_t pread64(int fd, void* buf, size_t nbytes, off64_t offset) {
    return pread(fd, buf, nbytes, offset);
}

static inline ssize_t pwrite64(int fd, const void* buf, size_t nbytes, off64_t offset) {
    return pwrite(fd, buf, nbytes, offset);
}

static inline int ftruncate64(int fd, off64_t length) {
    return ftruncate(fd, length);
}

#endif /* __APPLE__ */

#if defined(_WIN32)
#define O_CLOEXEC O_NOINHERIT
#define O_NOFOLLOW 0
#define DEFFILEMODE 0666
#endif /* _WIN32 */

#define ZD "%zd"
#define ZD_TYPE ssize_t

/*
 * Needed for cases where something should be constexpr if possible, but not
 * being constexpr is fine if in pre-C++11 code (such as a const static float
 * member variable).
 */
#if __cplusplus >= 201103L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif



#if defined(_WIN32)
#define OS_PATH_SEPARATOR '\\'
#else
#define OS_PATH_SEPARATOR '/'
#endif