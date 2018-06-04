
#include <builtin.h>
#include "internal/intrin.h"

#pragma intrinsic(_alloca)
#pragma intrinsic(abs)
#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#pragma intrinsic(__debugbreak)
#pragma intrinsic(labs)
#pragma intrinsic(_lrotl)
#pragma intrinsic(_lrotr)
#pragma intrinsic(memcmp)
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotr)
#pragma intrinsic(strcat)
#pragma intrinsic(strcmp)
#pragma intrinsic(strcpy)
#pragma intrinsic(strlen)
#pragma intrinsic(_strset)
#pragma intrinsic(strset)
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

#if (__MACHINEW64 == __MACHINEZ)
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
    unsigned char result = _BitScanForward64(&index , (unsigned __int64)x) ;
    return ((result == 0) ? 0 : (int)(index + 1)) ;
}

