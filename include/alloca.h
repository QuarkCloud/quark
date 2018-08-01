
#ifndef __QKC_ALLOCA_H
#define __QKC_ALLOCA_H 1

#include <quark_compile.h>
#include <builtin.h>

__BEGIN_DECLS

QUARK_LINKAGE void *_alloca (size_t size) ;

#ifdef  __GNUC__
# define alloca(size)   __builtin_alloca (size)
#else
# define alloca(size)   _alloca (size)
#endif /* GCC.  */

__END_DECLS

#endif /**__QKC_ALLOCA_H*/

