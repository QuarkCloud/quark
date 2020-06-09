
#ifndef __QUARK_WOBJS_BIT_OP_H
#define __QUARK_WOBJS_BIT_OP_H 1

#include <quark_compile.h>
#include <stdint.h>

namespace qkc {

	static inline bool BitOPIn(uint32_t val, uint32_t mask) { return ((val & mask) == mask); }
	static inline void BitOPSet(uint32_t& val, uint32_t mask) { val |= mask; }
	static inline uint32_t BitOPGet(uint32_t val, uint32_t mask) { return (val &= mask); }
	static inline void BitOPClear(uint32_t& val, uint32_t mask) { val &= ~mask; }
	static inline void BitOPReverse(uint32_t val, uint32_t mask) { val ^= mask; }

}

#endif  /** __QUARK_WOBJS_BIT_OP_H */
