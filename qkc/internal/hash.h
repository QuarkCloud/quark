
#ifndef __QKC_INTERNAL_HASH_H
#define __QKC_INTERNAL_HASH_H 1

#include <quark_compile.h>
#include <stdint.h>

__BEGIN_DECLS

QKCAPI uint32_t hash_time33(const char * str , int size , uint32_t hash) ;

__END_DECLS

#endif /** __QKC_INTERNAL_HASH_H */
