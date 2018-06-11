
#ifndef __QKC_BUILTIN_H
#define __QKC_BUILTIN_H 1

#include <quark_compile.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __thread

/**
    实现gcc内建函数，这在很多linux下源码用到，必须支持。
*/
QKCAPI int __builtin_ffs(int x)  ;
QKCAPI int __builtin_ffsl(long x)  ;
QKCAPI int __builtin_ffsll(long long x) ;

QKCAPI void __builtin_unreachable() ;
QKCAPI long __builtin_expect(long exp, long c);

#define __builtin_alloca    _alloca


static const int __ATOMIC_RELAXED = 1 ;    //No barriers or synchronization.
static const int __ATOMIC_CONSUME = 2 ;    //Data dependency only for both barrier and synchronization with another thread.
static const int __ATOMIC_ACQUIRE = 3 ;    //Barrier to hoisting of code and synchronizes with release (or stronger) semantic stores from another thread.
static const int __ATOMIC_RELEASE = 4 ;    //Barrier to sinking of code and synchronizes with acquire (or stronger) semantic loads from another thread.
static const int __ATOMIC_ACQ_REL = 5 ;    //Full barrier in both directions and synchronizes with acquire loads and release stores in another thread.
static const int __ATOMIC_SEQ_CST = 6 ;    //Full barrier in both directions and synchronizes with acquire loads and release stores in all threads. 

QKCAPI void __atomic_thread_fence(int mo) ;
QKCAPI void __atomic_load_impl(const void * ptr , void * ret , int memorder , size_t size) ;
QKCAPI void __atomic_store_impl(const void * ptr , void *ret , int memorder , size_t size) ;
QKCAPI void __atomic_exchange_impl(const void *ptr, void *val, void * ret, int memorder , size_t size) ;
QKCAPI bool __atomic_compare_exchange_impl(const void *ptr, void *expected, void *desired, 
                      bool weak, int success_memorder, int failure_memorder , size_t size) ;

QKCAPI void __atomic_fetch_add_impl(void *ptr, const void * val, int memorder, void *result, size_t size) ;
QKCAPI void __atomic_fetch_sub_impl(void *ptr, const void * val, int memorder, void *result, size_t size) ;
QKCAPI void __atomic_fetch_and_impl(void *ptr, const void * val, int memorder, void *result, size_t size) ;
QKCAPI void __atomic_fetch_xor_impl(void *ptr, const void * val, int memorder, void *result, size_t size) ;
QKCAPI void __atomic_fetch_or_impl(void *ptr, const void * val, int memorder, void *result, size_t size) ;


#ifdef __cplusplus
}
#endif

/**
    __atomic_xxx是泛型函数，和c函数是不一样的。
    包括其他内建函数，需要用C++的技术来实现
*/
#ifdef __cplusplus

template<typename type> 
void __atomic_load(const type * ptr , type *ret , int memorder) 
{
    __atomic_load_impl(ptr , ret , memorder , sizeof(type)) ;
}

template<typename type> 
void __atomic_store(const type * ptr , type *ret , int memorder) 
{
    __atomic_store_impl(ptr , ret , memorder , sizeof(type)) ;
}

template<typename type> 
void __atomic_exchange(const type *ptr, type *val, type *ret, int memorder) 
{
    __atomic_exchange_impl(ptr , val , ret , memorder , sizeof(type)) ;
}

template<typename type> 
bool __atomic_compare_exchange (const type *ptr, type *expected, type *desired, bool weak, int success_memorder, int failure_memorder)
{
    return __atomic_compare_exchange_impl(ptr , expected , desired , weak , success_memorder , failure_memorder , sizeof(type)) ;
}

template<typename type> 
type __atomic_fetch_add (type *ptr, const type val, int memorder)
{
    type result ;
    __atomic_fetch_add_impl(ptr , &val , memorder , &result , sizeof(type)) ;
    return result ;
}

template<typename type> 
type __atomic_fetch_sub (type *ptr, const type val, int memorder)
{
    type result ;
    __atomic_fetch_sub_impl(ptr , &val , memorder , &result , sizeof(type)) ;
    return result ;
}

template<typename type> 
type __atomic_fetch_and (type *ptr, const type val, int memorder)
{
    type result ;
    __atomic_fetch_and_impl(ptr , &val , memorder , &result , sizeof(type)) ;
    return result ;
}

template<typename type> 
type __atomic_fetch_xor (type *ptr, const type val, int memorder)
{
    type result ;
    __atomic_fetch_xor_impl(ptr , &val , memorder , &result , sizeof(type)) ;
    return result ;
}

template<typename type> 
type __atomic_fetch_or (type *ptr, const type val, int memorder)
{
    type result ;
    __atomic_fetch_or_impl(ptr , &val , memorder , &result , sizeof(type)) ;
    return result ;
}

#endif

#endif /* __QKC_BUILTIN_H */
