
#ifndef	__QUARK_ASSERT_H
#define __QUARK_ASSERT_H 1

#include <quark_compile.h>

__BEGIN_DECLS


QKCAPI void __assert_fail(const char *assertion, const char *file,  unsigned int line, const char *func);


__END_DECLS

#if defined(DEBUG)

    #define assert(expr)							                    \
        ((expr)								                            \
        ? __ASSERT_VOID_CAST (0)						                \
        : __assert_fail (#expr, __FILE__, __LINE__, __FUNCTION__))
#else
    #  define assert(expr)	
#endif


#endif  /** __QUARK_ASSERT_H */
