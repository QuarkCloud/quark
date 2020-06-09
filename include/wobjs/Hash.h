
#ifndef __QUARK_WOBJS_HASH_H
#define __QUARK_WOBJS_HASH_H 1

#include <quark_compile.h>
#include <stdint.h>

namespace qkc {

	QKCAPI uint32_t HashTime33(const char * str, int size, uint32_t hash);

}

#endif  /** __QUARK_WOBJS_HASH_H */
