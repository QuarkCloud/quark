
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

#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanForward64)




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

