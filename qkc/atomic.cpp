
#include <windows.h>
#include <atomic.h>
#include <bits/wordsize.h>

uint32_t atomic_load32(uint32_t volatile * target)
{
    return ::InterlockedCompareExchange((LONG volatile *)target , 0 , 0) ;
}

uint32_t atomic_store32(uint32_t volatile * target , uint32_t value)
{
    return ::InterlockedExchange((LONG volatile *)target , value) ;
}

uint32_t atomic_exchange32(uint32_t volatile * target, uint32_t value) 
{
    return ::InterlockedExchange((LONG volatile *)target , value) ;
}

uint32_t atomic_compare_exchange32(uint32_t volatile * target, uint32_t expected , uint32_t desired) 
{
    return ::InterlockedCompareExchange((LONG volatile *)target , desired , expected);
}

uint32_t atomic_fetch_add32(uint32_t volatile * target , uint32_t value)
{
    return ::InterlockedExchangeAdd((LONG volatile *)target , value) ;
}

uint32_t atomic_fetch_sub32(uint32_t volatile * target , uint32_t value)
{
    return ::InterlockedExchangeAdd((LONG volatile *)target , -(LONG)value) ;
}

uint32_t atomic_fetch_and32(uint32_t volatile * target , uint32_t value)
{
    return ::_InterlockedAnd((LONG volatile *)target , value) ;
}

uint32_t atomic_fetch_xor32(uint32_t volatile * target , uint32_t value)
{
    return ::_InterlockedXor((LONG volatile *)target , value) ;
}

uint32_t atomic_fetch_or32 (uint32_t volatile * target , uint32_t value)
{
    return ::_InterlockedOr((LONG volatile *)target , value) ;
}


#if __WORDSIZE == 32

static inline LONGLONG InterlockedExchange64(LONGLONG volatile * target , LONGLONG value)
{
    LONGLONG old = 0 ;
    do{
        old = (*target) ;
    }while(::InterlockedCompareExchange64(target , value , old) != old) ;

    return old ;
}

static inline LONGLONG InterlockedExchangeAdd64(LONGLONG volatile * target , LONGLONG value)
{
    LONGLONG old = 0 ;
    do{
        old = (*target) ;
    }while(::InterlockedCompareExchange64(target , (value + old) , old) != old) ;

    return old ;
}

static inline LONGLONG InterlockedAnd64(LONGLONG volatile * target , LONGLONG value)
{
    LONGLONG old = 0 ;
    do{
        old = (*target) ;
    }while(::InterlockedCompareExchange64(target , (value & old) , old) != old) ;

    return old ;
}

static inline LONGLONG InterlockedXor64(LONGLONG volatile * target , LONGLONG value)
{
    LONGLONG old = 0 ;
    do{
        old = (*target) ;
    }while(::InterlockedCompareExchange64(target , (value ^ old) , old) != old) ;

    return old ;
}

static inline LONGLONG InterlockedOr64(LONGLONG volatile * target , LONGLONG value)
{
    LONGLONG old = 0 ;
    do{
        old = (*target) ;
    }while(::InterlockedCompareExchange64(target , (value | old) , old) != old) ;

    return old ;
}

#elif defined(_AMD64_)

#define InterlockedExchange64		_InterlockedExchange64
#define InterlockedExchangeAdd64	_InterlockedExchangeAdd64
#define InterlockedAnd64			_InterlockedAnd64
#define InterlockedXor64			_InterlockedXor64
#define InterlockedOr64				_InterlockedOr64

#endif


uint64_t atomic_load64(uint64_t volatile * target)
{
    return ::InterlockedCompareExchange64((LONGLONG volatile *)target , 0 , 0) ;
}

uint64_t atomic_store64(uint64_t volatile * target , uint64_t value)
{
    return ::InterlockedExchange64((LONGLONG volatile *)target , value) ;
}

uint64_t atomic_exchange64(uint64_t volatile * target, uint64_t value)
{
    return ::InterlockedExchange64((LONGLONG volatile *)target , value) ;
}

uint64_t atomic_compare_exchange64(uint64_t volatile * target, uint64_t expected , uint64_t desired)
{
    return ::InterlockedCompareExchange64((LONGLONG volatile *)target , desired , expected) ;
}

uint64_t atomic_fetch_add64(uint64_t volatile * target , uint64_t value)
{
    return ::InterlockedExchangeAdd64((LONGLONG volatile *)target , value) ;
}

uint64_t atomic_fetch_sub64(uint64_t volatile * target , uint64_t value) 
{
    return ::InterlockedExchangeAdd64((LONGLONG volatile *)target , 0 - value) ;
}

uint64_t atomic_fetch_and64(uint64_t volatile * target , uint64_t value)
{
    return ::InterlockedAnd64((LONGLONG volatile *)target , value) ;
}

uint64_t atomic_fetch_xor64(uint64_t volatile * target , uint64_t value)
{
    return ::InterlockedXor64((LONGLONG volatile *)target , value) ;
}

uint64_t atomic_fetch_or64 (uint64_t volatile * target , uint64_t value)
{
    return ::InterlockedOr64((LONGLONG volatile *)target , value) ;
}

#if __WORDSIZE == 32

void * atomic_load_ptr(void * volatile * target)
{
    return (void *)::InterlockedCompareExchange((LONG volatile *)target , 0 , 0) ;
}

void * atomic_store_ptr(void * volatile * target , void * value)
{
    return (void *)::InterlockedExchange((LONG volatile *)target , (LONG)value) ;
}

void * atomic_exchange_ptr(void * volatile * target, void * value)
{
    return (void *)::InterlockedExchange((LONG volatile *)target , (LONG)value) ;
}

void * atomic_compare_exchange_ptr(void * volatile * target, void * expected , void * desired) 
{
    return (void *)::InterlockedCompareExchange((LONG volatile *)target , (LONG)desired , (LONG)expected) ;
}

#else


void * atomic_load_ptr(void * volatile * target)
{
    return (void *)::InterlockedCompareExchange64((LONGLONG volatile *)target , 0 , 0) ;
}

void * atomic_store_ptr(void * volatile * target , void * value)
{
    return (void *)::InterlockedExchange64((LONGLONG volatile *)target , (LONGLONG)value) ;
}

void * atomic_exchange_ptr(void * volatile * target, void * value)
{
    return (void *)::InterlockedExchange64((LONGLONG volatile *)target , (LONGLONG)value) ;
}

void * atomic_compare_exchange_ptr(void * volatile * target, void * expected , void * desired) 
{
    return (void *)::InterlockedCompareExchange64((LONGLONG volatile *)target , (LONGLONG)desired , (LONGLONG)expected) ;
}

#endif
