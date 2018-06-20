
#ifndef __QKC_ATOMIC_H
#define __QKC_ATOMIC_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
//32Œª
QKCAPI uint32_t atomic_load32(uint32_t volatile * target) ;
QKCAPI uint32_t atomic_store32(uint32_t volatile * target , uint32_t value) ;
QKCAPI uint32_t atomic_exchange32(uint32_t volatile * target, uint32_t value) ;
QKCAPI uint32_t atomic_compare_exchange32(uint32_t volatile * target, uint32_t expected , uint32_t desired) ;

QKCAPI uint32_t atomic_fetch_add32(uint32_t volatile * target , uint32_t value) ;
QKCAPI uint32_t atomic_fetch_sub32(uint32_t volatile * target , uint32_t value) ;
QKCAPI uint32_t atomic_fetch_and32(uint32_t volatile * target , uint32_t value) ;
QKCAPI uint32_t atomic_fetch_xor32(uint32_t volatile * target , uint32_t value) ;
QKCAPI uint32_t atomic_fetch_or32 (uint32_t volatile * target , uint32_t value) ;


//÷∏’Î
QKCAPI void * atomic_load_ptr(void * volatile * target) ;
QKCAPI void * atomic_store_ptr(void * volatile * target , void * value) ;
QKCAPI void * atomic_exchange_ptr(void * volatile * target, void * value) ;
QKCAPI void * atomic_compare_exchange_ptr(void * volatile * target, void * expected , void * desired) ;

//64Œª
QKCAPI uint64_t atomic_load64(uint64_t volatile * target) ;
QKCAPI uint64_t atomic_store64(uint64_t volatile * target , uint64_t value) ;
QKCAPI uint64_t atomic_exchange64(uint64_t volatile * target, uint64_t value) ;
QKCAPI uint64_t atomic_compare_exchange64(uint64_t volatile * target, uint64_t expected , uint64_t desired) ;

QKCAPI uint64_t atomic_fetch_add64(uint64_t volatile * target , uint64_t value) ;
QKCAPI uint64_t atomic_fetch_sub64(uint64_t volatile * target , uint64_t value) ;
QKCAPI uint64_t atomic_fetch_and64(uint64_t volatile * target , uint64_t value) ;
QKCAPI uint64_t atomic_fetch_xor64(uint64_t volatile * target , uint64_t value) ;
QKCAPI uint64_t atomic_fetch_or64 (uint64_t volatile * target , uint64_t value) ;

#ifdef __cplusplus
}
#endif


#endif /**__QKC_ATOMIC_H*/

