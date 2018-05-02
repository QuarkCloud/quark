
#ifndef __QKC_BITOP_H
#define __QKC_BITOP_H 1

#include <stdint.h>

static inline bool bitop_in(uint32_t val , uint32_t mask) {return ((val & mask) == mask) ;}
static inline void bitop_set(uint32_t val , uint32_t mask) {val |= mask ;}
static inline uint32_t bitop_get(uint32_t val , uint32_t mask) {return (val &= mask) ;}
static inline void bitop_clear(uint32_t val , uint32_t mask) {val &= ~mask ;}
static inline void bitop_reverse(uint32_t val , uint32_t mask) {val ^= mask ;}


#endif /** __QKC_BITOP_H */
