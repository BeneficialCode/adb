#pragma once

#include <stddef.h>


template <typename Func, typename... Args>
auto TEMP_FAILURE_RETRY(Func&& func,Args&& ... args) -> decltype(func(std::forward<Args>(args)...)) {
	decltype(func(std::forward<Args>(args)...)) _rc;
	do {
		_rc = func(std::forward<Args>(args)...);
	} while (_rc == -1 && errno == EINTR);
	return _rc;
}

#if __cplusplus >= 201103L
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif
// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)
// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
//
// One caveat is that arraysize() doesn't accept any array of an
// anonymous type or a type defined inside a function.  In these rare
// cases, you have to use the unsafe ARRAYSIZE_UNSAFE() macro below.  This is
// due to a limitation in C++'s template system.  The limitation might
// eventually be removed, but it hasn't happened yet.
// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];  // NOLINT(readability/casting)
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#define SIZEOF_MEMBER(t, f) sizeof(std::declval<t>().f)

// ARRAYSIZE_UNSAFE performs essentially the same calculation as arraysize,
// but can be used on anonymous types or types defined inside
// functions.  It's less safe than arraysize as it accepts some
// (although not all) pointers.  Therefore, you should use arraysize
// whenever possible.
//
// The expression ARRAYSIZE_UNSAFE(a) is a compile-time constant of type
// size_t.
//
// ARRAYSIZE_UNSAFE catches a few type errors.  If you see a compiler error
//
//   "warning: division by zero in ..."
//
// when using ARRAYSIZE_UNSAFE, you are (wrongfully) giving it a pointer.
// You should only use ARRAYSIZE_UNSAFE on statically allocated arrays.
//
// The following comments are on the implementation details, and can
// be ignored by the users.
//
// ARRAYSIZE_UNSAFE(arr) works by inspecting sizeof(arr) (the # of bytes in
// the array) and sizeof(*(arr)) (the # of bytes in one array
// element).  If the former is divisible by the latter, perhaps arr is
// indeed an array, in which case the division result is the # of
// elements in the array.  Otherwise, arr cannot possibly be an array,
// and we generate a compiler error to prevent the code from
// compiling.
//
// Since the size of bool is implementation-defined, we need to cast
// !(sizeof(a) & sizeof(*(a))) to size_t in order to ensure the final
// result has type size_t.
//
// This macro is not perfect as it wrongfully accepts certain
// pointers, namely where the pointer size is divisible by the pointee
// size.  Since all our code has to go through a 32-bit compiler,
// where a pointer is 4 bytes, this means all pointers to a type whose
// size is 3 or greater than 4 will be (righteously) rejected.
#define ARRAYSIZE_UNSAFE(a)     \
  ((sizeof(a) / sizeof(*(a))) / \
    static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
// Changing this definition will cause you a lot of pain.  A majority of
// vendor code defines LIKELY and UNLIKELY this way, and includes
// this header through an indirect path.
#define LIKELY( exp )       ( (exp) != 0)
#define UNLIKELY( exp )     ( (exp) != 0)
#define WARN_UNUSED __attribute__((warn_unused_result))
// A deprecated function to call to create a false use of the parameter, for
// example:
//   int foo(int x) { UNUSED(x); return 10; }
// to avoid compiler warnings. Going forward we prefer ATTRIBUTE_UNUSED.
template <typename... T>
void UNUSED(const T&...) {
}
// An attribute to place on a parameter to a function, for example:
//   int foo(int x ATTRIBUTE_UNUSED) { return 10; }
// to avoid compiler warnings.
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
// The FALLTHROUGH_INTENDED macro can be used to annotate implicit fall-through
// between switch labels:
//  switch (x) {
//    case 40:
//    case 41:
//      if (truth_is_out_there) {
//        ++x;
//        FALLTHROUGH_INTENDED;  // Use instead of/along with annotations in
//                               // comments.
//      } else {
//        return x;
//      }
//    case 42:
//      ...
//
//  As shown in the example above, the FALLTHROUGH_INTENDED macro should be
//  followed by a semicolon. It is designed to mimic control-flow statements
//  like 'break;', so it can be placed in most places where 'break;' can, but
//  only if there are no statements on the execution path between it and the
//  next switch label.
//
//  When compiled with clang in C++11 mode, the FALLTHROUGH_INTENDED macro is
//  expanded to [[clang::fallthrough]] attribute, which is analysed when
//  performing switch labels fall-through diagnostic ('-Wimplicit-fallthrough').
//  See clang documentation on language extensions for details:
//  http://clang.llvm.org/docs/LanguageExtensions.html#clang__fallthrough
//
//  When used with unsupported compilers, the FALLTHROUGH_INTENDED macro has no
//  effect on diagnostics.
//
//  In either case this macro has no effect on runtime behavior and performance
//  of code.
#ifndef FALLTHROUGH_INTENDED
#define FALLTHROUGH_INTENDED \
  do {                       \
  } while (0)
#endif
