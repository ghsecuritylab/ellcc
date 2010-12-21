#ifndef _CONFIG_H_

/* libecc configuration parameters.
 */

/* Define if reentrant system calls are provided.
 */
#define REENTRANT_SYSCALLS_PROVIDED

/** Define for a single threaded environment.
 */
// #define __SINGLE_THREAD__
/** Define if the stat structure has blksize.
 */
#define HAVE_BLKSIZE
/** Enable multi-byte characters.
 */
// #define _MB_CAPABLE
/** Define for formatting with positional arguments.
 */
// #define _WANT_IO_POS_ARGS
/** Define for formatting with long longs.
 */
#define _WANT_IO_LONG_LONG
/** Define for formatting with long doubles.
 */
// #define _WANT_IO_LONG_DOUBLE
/** Define for formatting with C99 semantics.
 */
#define _WANT_IO_C99_FORMATS
/** Build a compact version of ctype.
 */
// #define COMPACT_CTYPE
/** Define if the nanosleep function exists.
 */
#define HAVE_NANOSLEEP
/** Define if _system exists elsewhere.
 */
// #define HAVE_SYSTEM
/** Define if exec does not exist.
 */
// #define NO_EXEC
/** Define if getopt exists elsewhere.
 */
// #define HAVE_GETOPT
/** Define atexit idata should be freed on exit.
 */
// #define _ATEXIT_DYNAMIC_ALLOC
/** Define if malloc does not exist or exists elsewhere.
 */
// #define MALLOC_PROVIDED

/** Define if opendir does not exist or exists elsewhere.
 */
#define HAVE_OPENDIR
/** Define if locale should be used.
 */
#define USE_LOCALE
/** Define if dirent contains d_namelen.
 */
// #define _DIRENT_HAVE_D_NAMLEN

/** Define if reentrant system calls are provided outside of libecc.
 */
// #define REENTRANT_SYSCALLS_PROVIDED
/** Build a small version of reentrant functions.
 */
// #define _REENT_SMALL
/** Define if reentrant system calls are provided outside of libecc.
 */
// #define REENTRANT_SYSCALLS_PROVIDED
/** Define if a DIR has a loc.
 */
// #define HAVE_DD_LOCK



#ifdef __cplusplus

/** Define to use the USTL standard template library.
 */
#define USE_USTL

#ifdef USE_USTL
// Definitions for the USTL library.

// Define to the one symbol short name of this package.
#define USTL_NAME	"ustl"
// Define to the full name and version of this package.
#define USTL_STRING	"ustl v1.3"
// Define to the version of this package.
#define USTL_VERSION	v1.3
// Define to the address where bug reports for this package should be sent.
#define USTL_BUGREPORT	"Rich Pennington <rich@pennware.com>"

/// Define to 1 if you want stream operations to throw exceptions on
/// insufficient data or insufficient space. All these errors should
/// be preventable in output code; the input code should verify the
/// data in a separate step. It slows down stream operations a lot,
/// but it is your decision. By default only debug builds throw.
///
#define WANT_STREAM_BOUNDS_CHECKING 1

#if !defined(WANT_STREAM_BOUNDS_CHECKING) && !defined(NDEBUG)
    #define WANT_STREAM_BOUNDS_CHECKING 1
#endif

/// Define to 1 if you want backtrace symbols demangled.
/// This adds some 15k to the library size, and requires that you link it and
/// any executables you make with the -rdynamic flag (increasing library size
/// even more). By default only the debug build does this.
#undef WANT_NAME_DEMANGLING

#if !defined(WANT_NAME_DEMANGLING) && !defined(NDEBUG)
    #define WANT_NAME_DEMANGLING 1
#endif

/// Define to 1 if you want to build without libstdc++
#define WITHOUT_LIBSTDCPP 1

/// Define GNU extensions if unavailable.
#ifndef __GNUC__
    /// GCC (and some other compilers) define '__attribute__'; ustl is using this
    /// macro to alert the compiler to flag inconsistencies in printf/scanf-like
    /// function calls.  Just in case '__attribute__' is undefined, make a dummy.
    /// 
    #ifndef __attribute__
	#define __attribute__(p)
    #endif
#endif
#if defined(__GNUC__) && __GNUC__ >= 4
    #define DLL_EXPORT		__attribute__((visibility("default")))
    #define DLL_LOCAL		__attribute__((visibility("hidden")))
    #define INLINE		__attribute__((always_inline))
#else
    #define DLL_EXPORT
    #define DLL_LOCAL
    #define INLINE
#endif
#if defined(__GNUC__) && __GNUC__ >= 3 && (__i386__ || __x86_64__)
    /// GCC 3+ supports the prefetch directive, which some CPUs use to improve caching
    #define prefetch(p,rw,loc)	__builtin_prefetch(p,rw,loc)
#else
    #define prefetch(p,rw,loc)
#endif
#if !defined(__GNUC__) || __GNUC__ < 3
    /// __alignof__ returns the recommended alignment for the type
    #define __alignof__(v)	min(sizeof(v), sizeof(void*))
    /// This macro returns 1 if the value of x is known at compile time.
    #ifndef __builtin_constant_p
	#define __builtin_constant_p(x)	0
    #endif
#endif

/// Define to 1 if you have the <assert.h> header file.
#define HAVE_ASSERT_H 1

/// Define to 1 if you have the <ctype.h> header file.
#define HAVE_CTYPE_H 1

/// Define to 1 if you have the <errno.h> header file.
#define HAVE_ERRNO_H 1

/// Define to 1 if you have the <fcntl.h> header file.
#define HAVE_FCNTL_H 1

/// Define to 1 if you have the <float.h> header file.
#define HAVE_FLOAT_H 1

/// Define to 1 if you have the <inttypes.h> header file.
#define HAVE_INTTYPES_H 1

/// Define to 1 if you have the <limits.h> header file.
#define HAVE_LIMITS_H 1

/// Define to 1 if you have the <locale.h> header file.
#define HAVE_LOCALE_H 1

// Define to 1 if you have the <alloca.h> header file.
#define HAVE_ALLOCA_H 1

// Define to 1 if you have the <signal.h> header file.
#define HAVE_SIGNAL_H 1

// Define to 1 if you have the __va_copy function
#define HAVE_VA_COPY 1

// Define to 1 if you have the <stdarg.h> header file.
#define HAVE_STDARG_H 1

// Define to 1 if you have the <stddef.h> header file.
#define HAVE_STDDEF_H 1

// Define to 1 if you have the <stdint.h> header file.
#define HAVE_STDINT_H 1

// Define to 1 if you have the <stdio.h> header file.
#define HAVE_STDIO_H 1

// Define to 1 if you have the <stdlib.h> header file.
#define HAVE_STDLIB_H 1

// Define to 1 if you have the <string.h> header file.
#define HAVE_STRING_H 1

// Define to 1 if you have the 'strrchr' function.
#define HAVE_STRRCHR 1

// Define to 1 if you have the 'strsignal' function.
#define HAVE_STRSIGNAL 1

// Define to 1 if you have the <sys/stat.h> header file.
#define HAVE_SYS_STAT_H 1

// Define to 1 if you have the <sys/types.h> header file.
#define HAVE_SYS_TYPES_H 1

// Define to 1 if you have the <time.h> header file.
#define HAVE_TIME_H 1

// Define to 1 if you have the <unistd.h> header file.
#define HAVE_UNISTD_H 1

// Define to 1 if you have the <math.h> header file.
#define HAVE_MATH_H 1

// Define to 1 if you have the <cxxabi.h> header file.
#undef HAVE_CXXABI_H

// Define to 1 if you have the rintf function. Will use rint otherwise.
#undef HAVE_RINTF

// STDC_HEADERS is defined to 1 on sane systems.
#if defined(HAVE_ASSERT_H) && defined(HAVE_CTYPE_H) &&\
    defined(HAVE_ERRNO_H) && defined(HAVE_FLOAT_H) &&\
    defined(HAVE_LIMITS_H) && defined(HAVE_LOCALE_H) &&\
    defined(HAVE_MATH_H) && defined(HAVE_SIGNAL_H) &&\
    defined(HAVE_STDARG_H) && defined(HAVE_STDDEF_H) &&\
    defined(HAVE_STDIO_H) && defined(HAVE_STDLIB_H) &&\
    defined(HAVE_STRING_H) && defined(HAVE_TIME_H)
#define STDC_HEADERS 1
#endif

// STDC_HEADERS is defined to 1 on unix systems.
#if defined(HAVE_FCNTL_H) && defined(HAVE_SYS_STAT_H) && defined(HAVE_UNISTD_H)
#define STDUNIX_HEADERS 1
#endif

// Define to 1 if your compiler treats char as a separate type along with
// signed char and unsigned char. This will create overloads for char.
#undef HAVE_THREE_CHAR_TYPES

// Define to 1 if you have 64 bit types available
#define HAVE_INT64_T 1

// Define to 1 if you have the long long type
#define HAVE_LONG_LONG 1

// Define to 1 if you want unrolled specializations for fill and copy
#define WANT_UNROLLED_COPY 1

// Define to 1 if you want to use MMX/SSE/3dNow! processor instructions
#define WANT_MMX 1

// Define to byte sizes of types
#define SIZE_OF_CHAR            __CHAR_SIZE__
#define SIZE_OF_SHORT           __SHORT_SIZE__
#define SIZE_OF_INT             __INT_SIZE__
#define SIZE_OF_LONG            __LONG_SIZE__
#define SIZE_OF_LONG_LONG       __LLONG_SIZE__
#define SIZE_OF_POINTER         __PTR_SIZE__
#define SIZE_OF_SIZE_T          __SIZE_TYPE_SIZE__
#define SIZE_OF_BOOL            __BOOL_SIZE__
#define SIZE_T_IS_LONG          (__SIZE_TYPE_SIZE__ == __LONG_SIZE__)

// Byte order macros, converted in utypes.h
#define USTL_LITTLE_ENDIAN	4321
#define USTL_BIG_ENDIAN		1234
#ifdef __LITTLE_ENDIAN__
#define USTL_BYTE_ORDER		USTL_LITTLE_ENDIAN
#endif
#ifdef __BIG_ENDIAN__
#define USTL_BYTE_ORDER		USTL_BIG_ENDIAN
#endif

// Extended CPU capabilities
// RICH
#undef CPU_HAS_FPU
#undef CPU_HAS_EXT_DEBUG
#undef CPU_HAS_TIMESTAMPC
#undef CPU_HAS_MSR
#undef CPU_HAS_CMPXCHG8
#undef CPU_HAS_APIC
#undef CPU_HAS_SYSCALL
#undef CPU_HAS_MTRR
#undef CPU_HAS_CMOV
#undef CPU_HAS_FCMOV
#if WANT_MMX
#undef CPU_HAS_MMX
#undef CPU_HAS_FXSAVE
#undef CPU_HAS_SSE 
#undef CPU_HAS_SSE2
#undef CPU_HAS_SSE3
#undef CPU_HAS_EXT_3DNOW
#undef CPU_HAS_3DNOW
#endif

// GCC vector extensions
#if (defined(CPU_HAS_MMX) || defined(CPU_HAS_SSE)) && __GNUC__ >= 3
    #undef HAVE_VECTOR_EXTENSIONS
#endif

#if CPU_HAS_SSE && defined(__GNUC__)
    #define __sse_align	__attribute__((aligned(16)))
#else
    #define __sse_align	
#endif

// Define to empty if 'const' does not conform to ANSI C.
#undef const
// Define as '__inline' if that is what the C compiler calls it
#undef inline
// Define to 'long' if <sys/types.h> does not define.
#undef off_t
// Define to 'unsigned' if <sys/types.h> does not define.
#undef size_t

#endif  // USE_USTL

#endif  // __cpluscplus

#endif
