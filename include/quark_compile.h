
#ifndef __QUARK_COMPILE_H
#define __QUARK_COMPILE_H 1

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#if (defined(__i386__) || defined(__amd64__) || defined(__x86_64__))
#define __i386__

#define __GNUC__
//#define __STDC_VERSION__  199409L
#define __asm__ __asm

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef QUARK_IMPORT
#define QUARK_IMPORT   __declspec(dllimport)
#endif

#ifndef QUARK_EXPORT
#define QUARK_EXPORT   __declspec(dllexport)
#endif

#ifndef QUARK_LINKAGE
//适用于用于导入第三库，而不是本地实现，但本地需要提供接口的函数
#define QUARK_LINKAGE   QUARK_IMPORT
#endif

#ifdef QKC_EXPORTS
	#define QKCAPI QUARK_EXPORT
#else
    #define QKCAPI QUARK_IMPORT
#endif

#ifdef QKN_EXPORTS
	#define QKNAPI QUARK_EXPORT
#else
    #define QKNAPI QUARK_IMPORT
#endif

#endif  /** __QUARK_COMPILE_H */
