
#ifndef __QKC_STDINT_H
#define __QKC_STDINT_H 1

#include <quark_compile.h>

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

typedef unsigned int    size_t ;


#endif  /** __QKC_STDINT_H */
