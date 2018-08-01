
#ifndef __QKC_BYTESWAP_H
#define __QKC_BYTESWAP_H 1

#include <quark_compile.h>
#include <stdint.h>

__BEGIN_DECLS

QKCAPI uint16_t bswap_16 (uint16_t x) ;
QKCAPI uint32_t bswap_32 (uint32_t x) ;
QKCAPI uint64_t bswap_64 (uint64_t x) ;

__END_DECLS

#endif /* __QKC_BYTESWAP_H */
