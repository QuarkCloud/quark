
#ifndef __QKC_ALLOCA_H
#define __QKC_ALLOCA_H 1

#include <quark_compile.h>
#include <builtin.h>

#ifdef __cplusplus
extern "C" {
#endif

QUARK_LINKAGE void *_alloca (size_t size) ;

#ifdef  __GNUC__
# define alloca(size)   __builtin_alloca (size)
#else
# define alloca(size)   _alloca (size)
#endif /* GCC.  */

#ifdef __cplusplus
}
#endif


#endif /**__QKC_ALLOCA_H*/

