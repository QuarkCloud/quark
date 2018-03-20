
#ifndef __QKC_INNER_HASH_H
#define __QKC_INNER_HASH_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

QKCAPI uint32_t hash_time33(const char * str , int size , uint32_t hash) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INNER_HASH_H */
