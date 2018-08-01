
#ifndef __QKC_STDARG_H
#define __QKC_STDARG_H 1

#include <quark_compile.h>

__BEGIN_DECLS

#ifndef _VA_LIST_DEFINED
typedef char *  va_list;
#define _VA_LIST_DEFINED
#endif  /* _VA_LIST_DEFINED */

#ifdef __cplusplus
#define _ADDRESSOF(v)   ( &reinterpret_cast<const char &>(v) )
#else
#define _ADDRESSOF(v)   ( &(v) )
#endif 

#define _SLOTSIZEOF(t)   (sizeof(t))
#define _APALIGN(t,ap)  (__alignof(t))

#if defined (_M_IX86)

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define _crt_va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define _crt_va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define _crt_va_end(ap)      ( ap = (va_list)0 )

#elif defined (_M_AMD64)
extern void __cdecl __va_start(va_list *, ...) ;

#define _crt_va_start(ap, x) ( __va_start(&ap, x) )
#define _crt_va_arg(ap, t)   \
    ( ( sizeof(t) > sizeof(__int64) || ( sizeof(t) & (sizeof(t) - 1) ) != 0 ) \
        ? **(t **)( ( ap += sizeof(__int64) ) - sizeof(__int64) ) \
        :  *(t  *)( ( ap += sizeof(__int64) ) - sizeof(__int64) ) )
#define _crt_va_end(ap)      ( ap = (va_list)0 )
#endif

#define va_start _crt_va_start
#define va_arg _crt_va_arg
#define va_end _crt_va_end

__END_DECLS

#endif  /* __QKC_STDARG_H */
