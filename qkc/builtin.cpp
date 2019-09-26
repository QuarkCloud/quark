
#include <builtin.h>
#include <stdlib.h>
#include <windows.h>

#pragma intrinsic(_alloca)
#pragma intrinsic(abs)
#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#pragma intrinsic(__debugbreak)
#pragma intrinsic(labs)
#pragma intrinsic(_lrotl)
#pragma intrinsic(_lrotr)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotr)

/***
#pragma intrinsic(memcmp)
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
*/

/***
#pragma intrinsic(strcat)
#pragma intrinsic(strcmp)
#pragma intrinsic(strcpy)
#pragma intrinsic(strlen)
#pragma intrinsic(_strset)
#pragma intrinsic(strset)
*/
#pragma intrinsic(_rotl64)
#pragma intrinsic(_rotr64)
#pragma intrinsic(_abs64)

#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedOr8)
#pragma intrinsic(_InterlockedOr16)
#pragma intrinsic(_InterlockedXor)
#pragma intrinsic(_InterlockedXor8)
#pragma intrinsic(_InterlockedXor16)
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedAnd8)
#pragma intrinsic(_InterlockedAnd16)
#pragma intrinsic(_bittest)
#pragma intrinsic(_bittestandset)
#pragma intrinsic(_bittestandreset)
#pragma intrinsic(_bittestandcomplement)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#pragma intrinsic(wcscat)
#pragma intrinsic(wcscmp)
#pragma intrinsic(wcscpy)
#pragma intrinsic(wcslen)
#pragma intrinsic(_wcsset)
#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_rotr8)
#pragma intrinsic(_rotr16)
#pragma intrinsic(_rotl8)
#pragma intrinsic(_rotl16)
#pragma intrinsic(_InterlockedIncrement16)
#pragma intrinsic(_InterlockedDecrement16)
#pragma intrinsic(_InterlockedCompareExchange16)

#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedCompareExchange)

#pragma intrinsic(_InterlockedCompareExchange64)

#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)


#if defined(_X86_) || defined(_M_IX86)
unsigned char _BitScanForward64(unsigned long* index, unsigned __int64 mask)
{
    if(index == NULL)    return 0 ;
    unsigned long * p32 = (unsigned long *)&mask ;
    unsigned char result = _BitScanForward(index , *p32) ;
    if(result != 0)
        return (unsigned char)((*index) + 1) ;
    if(sizeof(unsigned long) != sizeof(unsigned __int64))
    {
        result = _BitScanForward(index , p32[1]) ;
        if(result != 0)
            return (unsigned char)((*index) + 9) ;
    }
    return 0 ;
}
#endif

#define BitScanForward64		_BitScanForward64


int __builtin_ffs (int x) 
{ 
    unsigned long index = 0 ;
    unsigned char result = _BitScanForward(&index , (unsigned long)x) ;
    return ((result == 0) ? 0 : (int)(index + 1)) ;
}

int __builtin_ffsl(long x) 
{
    unsigned long index = 0 ;
    unsigned char result = _BitScanForward(&index , (unsigned long)x) ;
    return ((result == 0) ? 0 : (int)(index + 1)) ;
}

int __builtin_ffsll (long long x)
{
    unsigned long index = 0 ;
    unsigned char result = BitScanForward64(&index , (unsigned __int64)x) ;
    return ((result == 0) ? 0 : (int)(index + 1)) ;
}

long __builtin_expect(long exp, long c)
{
    return exp ;
}

void __builtin_unreachable() 
{
    ::abort() ;
}

void __atomic_thread_fence(int mo) 
{
    _ReadWriteBarrier() ;
#if defined(_M_IX86) || defined(_M_X64)
    if(mo != __ATOMIC_RELAXED)
        ::_WriteBarrier() ;
#endif
    _ReadWriteBarrier() ;
}


void * __builtin_return_address(unsigned int level)
{
    void * stack[1] = {NULL} ;
    DWORD depth = ::RtlCaptureStackBackTrace(level , 1 , stack , NULL) ;
    if(depth == 0)
        return NULL;
    else
        return stack[0] ;
}

void * __builtin_frame_address(unsigned int level)
{
    void * stack[1] = {NULL} ;
    DWORD depth = ::RtlCaptureStackBackTrace(level , 1 , stack , NULL) ;
    if(depth == 0)
        return NULL;
    else
        return stack[0] ;
}



