#ifndef __QKC_LIMITS_H
#define __QKC_LIMITS_H 1

#include <quark_compile.h>

#  define CHAR_BIT	8

#  define SCHAR_MIN	(-128)
#  define SCHAR_MAX	127

#  define UCHAR_MAX	255

#  ifdef __CHAR_UNSIGNED__
#   define CHAR_MIN	0
#   define CHAR_MAX	UCHAR_MAX
#  else
#   define CHAR_MIN	SCHAR_MIN
#   define CHAR_MAX	SCHAR_MAX
#  endif

/* Minimum and maximum values a `signed short int' can hold.  */
#  define SHRT_MIN	(-32768)
#  define SHRT_MAX	32767

/* Maximum value an `unsigned short int' can hold.  (Minimum is 0.)  */
#  define USHRT_MAX	65535

/* Minimum and maximum values a `signed int' can hold.  */
#  define INT_MIN	(-INT_MAX - 1)
#  define INT_MAX	2147483647

/* Maximum value an `unsigned int' can hold.  (Minimum is 0.)  */
#  define UINT_MAX	4294967295U

/* Minimum and maximum values a `signed long int' can hold.  */
#  if __WORDSIZE == 64
#   define LONG_MAX	9223372036854775807L
#  else
#   define LONG_MAX	2147483647L
#  endif
#  define LONG_MIN	(-LONG_MAX - 1L)

/* Maximum value an `unsigned long int' can hold.  (Minimum is 0.)  */
#  if __WORDSIZE == 64
#   define ULONG_MAX	18446744073709551615UL
#  else
#   define ULONG_MAX	4294967295UL
#  endif

/* Minimum and maximum values a `signed long long int' can hold.  */
#   define LLONG_MAX	9223372036854775807LL
#   define LLONG_MIN	(-LLONG_MAX - 1LL)

/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
#   define ULLONG_MAX	18446744073709551615ULL



#define NR_OPEN         1024

#define NGROUPS_MAX    65536    /* supplemental group IDs are available */
#define ARG_MAX       131072    /* # bytes of args + environ for exec() */
#define CHILD_MAX        999    /* no limit :-) */
#define OPEN_MAX         256    /* # open files a process may have */
#define LINK_MAX         127    /* # links a file may have */
#define MAX_CANON        255    /* size of the canonical input queue */
#define MAX_INPUT        255    /* size of the type-ahead buffer */
#define NAME_MAX         255    /* # chars in a file name */
#define PATH_MAX        4096    /* # chars in a path name including nul */
#define PIPE_BUF        4096    /* # bytes in atomic write to a pipe */
#define XATTR_NAME_MAX   255    /* # chars in an extended attribute name */
#define XATTR_SIZE_MAX 65536    /* size of an extended attribute value (64k) */
#define XATTR_LIST_MAX 65536    /* size of extended attribute namelist (64k) */

#define RTSIG_MAX         32



#endif /**__QKC_LIMITS_H*/
