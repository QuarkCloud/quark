
#ifndef __QKC_BUILTIN_H
#define __QKC_BUILTIN_H 1

#include <quark_compile.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
    ʵ��gcc�ڽ����������ںܶ�linux��Դ���õ�������֧�֡�
*/
QKCAPI int __builtin_ffs (int x)  ;
QKCAPI int __builtin_ffsl(long x)  ;
QKCAPI int __builtin_ffsll (long long x) ;

static const int __ATOMIC_RELAXED = 1 ;    //No barriers or synchronization.
static const int __ATOMIC_CONSUME = 2 ;    //Data dependency only for both barrier and synchronization with another thread.
static const int __ATOMIC_ACQUIRE = 3 ;    //Barrier to hoisting of code and synchronizes with release (or stronger) semantic stores from another thread.
static const int __ATOMIC_RELEASE = 4 ;    //Barrier to sinking of code and synchronizes with acquire (or stronger) semantic loads from another thread.
static const int __ATOMIC_ACQ_REL = 5 ;    //Full barrier in both directions and synchronizes with acquire loads and release stores in another thread.
static const int __ATOMIC_SEQ_CST = 6 ;    //Full barrier in both directions and synchronizes with acquire loads and release stores in all threads. 

#ifdef __cplusplus
}
#endif


#endif /* __QKC_BUILTIN_H */