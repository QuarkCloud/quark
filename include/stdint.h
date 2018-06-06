
#ifndef __QKC_STDINT_H
#define __QKC_STDINT_H 1

#include <bits/wordsize.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef char                    int8_t    ;
typedef unsigned char           uint8_t   ;
typedef short int               int16_t   ;
typedef unsigned short int      uint16_t  ;
typedef int                     int32_t   ;
typedef unsigned int            uint32_t  ;

typedef __int64                 int64_t   ;
typedef unsigned __int64        uint64_t  ;

#if __WORDSIZE == 32
typedef int32_t                 intptr_t  ;
typedef uint32_t                uintptr_t ;
#else
typedef int64_t                 intptr_t  ;
typedef uint64_t                uintptr_t ;
#endif

typedef int64_t                 intmax_t ;
typedef uint64_t                uintmax_t ;


/* Signed.  */
# define INT8_C(c)      c
# define INT16_C(c)     c
# define INT32_C(c)     c
# if __WORDSIZE == 64
#  define INT64_C(c)    c ## L
# else
#  define INT64_C(c)    c ## LL
# endif

/* Unsigned.  */
# define UINT8_C(c)     c
# define UINT16_C(c)    c
# define UINT32_C(c)    c ## U
# if __WORDSIZE == 64
#  define UINT64_C(c)   c ## UL
# else
#  define UINT64_C(c)   c ## ULL
# endif

/* Maximal type.  */
# if __WORDSIZE == 64
#  define INTMAX_C(c)   c ## L
#  define UINTMAX_C(c)  c ## UL
# else
#  define INTMAX_C(c)   c ## LL
#  define UINTMAX_C(c)  c ## ULL
# endif


# if __WORDSIZE == 64
#  define SIZE_MAX              (18446744073709551615UL)
# else
#  define SIZE_MAX              (4294967295U)
# endif

# if __WORDSIZE == 64
#  define __INT64_C(c)  c ## L
#  define __UINT64_C(c) c ## UL
# else
#  define __INT64_C(c)  c ## LL
#  define __UINT64_C(c) c ## ULL
# endif


/* Minimum of signed integral types.  */
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647-1)
# define INT64_MIN              (-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types.  */
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647)
# define INT64_MAX              (__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types.  */
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295U)
# define UINT64_MAX             (__UINT64_C(18446744073709551615))



#ifdef __cplusplus
}
#endif

#endif  /** __QKC_STDINT_H */
