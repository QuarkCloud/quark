
#ifndef __QKC_WSTUB_INTRIN_H
#define __QKC_WSTUB_INTRIN_H 1

#include <quark_compile.h>

#define _CRTIMP QUARK_IMPORT

__BEGIN_DECLS

/*
** __MACHINE             : all compilers
** __MACHINEI            : Intel (32 bit x86) and X64
** __MACHINEX64          : X64 compiler only
** __MACHINEIA32         : 32 bit x86 arch only
** __MACHINEX86X_X64     : X86 Extended intrinsics supported on X64
** __MACHINEX86X_IA64    : X86 Extended intrinsics supported on IA64
** __MACHINEIA64         : IA64 compiler only
** __MACHINEW64          : WIN64(tm), 64 bit compilers only
** __MACHINEIW64         : IA32 + Win64 compilers only (__MACHINEI + __MACHINEW64)
** __MACHINESA           : ARM (StrongARM) only
** __MACHINEARMX         : ARM XSCALE intrinsics
** __MACHINEZ            : nothing
*/

#define __MACHINEI            __MACHINE
#define __MACHINEX64          __MACHINE
#define __MACHINEIA32         __MACHINE
#define __MACHINEX86X_X64     __MACHINE
#define __MACHINEX86X_IA64    __MACHINE
#define __MACHINEIA64         __MACHINE
#define __MACHINEW64          __MACHINE
#define __MACHINEIW64         __MACHINE
#define __MACHINESA           __MACHINE
#define __MACHINEARMX         __MACHINE

#define __MACHINE(X)          X;
#define __MACHINEZ(X)         /* NOTHING */

#if !(_M_IX86)
#undef __MACHINEIA32
#define __MACHINEIA32         __MACHINEZ
#endif

#if !(_M_IA64)
#undef __MACHINEIA64
#define __MACHINEIA64         __MACHINEZ
#endif

#if !(_M_AMD64)
#undef __MACHINEX64
#define __MACHINEX64          __MACHINEZ
#endif

#if !(_M_IA64 || _M_AMD64)
#undef __MACHINEW64
#define __MACHINEW64          __MACHINEZ
#endif

#if !(_M_IX86 || _M_AMD64 || _M_IA64)
#undef __MACHINEIW64
#define __MACHINEIW64         __MACHINEZ
#endif

#if !(_M_IX86 || _M_IA64)
#undef __MACHINEX86X_IA64
#define __MACHINEX86X_IA64    __MACHINEZ
#endif

#if !(_M_IX86 || _M_AMD64)
#undef __MACHINEX86X_X64
#define __MACHINEX86X_X64     __MACHINEZ
#endif

#if !(_M_ARM)
#undef  __MACHINESA
#undef  __MACHINEARMX
#define __MACHINESA           __MACHINEZ
#define __MACHINEARMX         __MACHINEZ
#endif

__MACHINEIA64(void _AcquireSpinLock(unsigned __int64 *))
__MACHINE(void * __cdecl _alloca(size_t))
__MACHINE(int __cdecl abs(int))
__MACHINEIA64(void __break(int))

__MACHINE(unsigned short __cdecl _byteswap_ushort(unsigned short value))
__MACHINE(unsigned long __cdecl _byteswap_ulong(unsigned long value))
__MACHINE(unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64 value))
__MACHINEX64(_CRTIMP double ceil(double))

__MACHINE(void __cdecl __debugbreak(void))
__MACHINEI(void __cdecl _disable(void))
__MACHINEIA64(void __cdecl _disable(void))
__MACHINEIA64(void __dsrlz(void))
__MACHINEI(__int64 __emul(int,int))
__MACHINEI(unsigned __int64 __emulu(unsigned int,unsigned int))
__MACHINEI(void __cdecl _enable(void))
__MACHINEIA64(void __cdecl _enable(void))
__MACHINEIA64(void __fc(__int64))
__MACHINEIA64(void __fclrf(void))
__MACHINEIA64(void __fsetc(int, int))
__MACHINEIA64(void __fwb(void))

__MACHINEIA64(long _InterlockedAdd(long volatile *, long))
__MACHINEIA64(long _InterlockedAdd_acq(long volatile *, long))
__MACHINEIA64(long _InterlockedAdd_rel(long volatile *, long))
__MACHINEIA64(__int64 _InterlockedAdd64(__int64 volatile *, __int64))
__MACHINEIA64(__int64 _InterlockedAdd64_acq(__int64 volatile *, __int64))
__MACHINEIA64(__int64 _InterlockedAdd64_rel(__int64 volatile *, __int64))
__MACHINEI(long __cdecl _InterlockedDecrement(long volatile *))
__MACHINEIA64(long _InterlockedDecrement(long volatile *))
__MACHINEIA64(long _InterlockedDecrement_acq(long volatile *))
__MACHINEIA64(long _InterlockedDecrement_rel(long volatile *))
__MACHINEIA64(__int64 _InterlockedDecrement64(__int64 volatile *))
__MACHINEIA64(__int64 _InterlockedDecrement64_acq(__int64 volatile *))
__MACHINEIA64(__int64 _InterlockedDecrement64_rel(__int64 volatile *))
__MACHINEX64(__int64 _InterlockedDecrement64(__int64 volatile *))
__MACHINEI(long _InterlockedExchange(long volatile *, long))
__MACHINEIA64(long _InterlockedExchange(long volatile *, long))
__MACHINEIA64(long _InterlockedExchange_acq(long volatile *, long))
__MACHINESA(long __stdcall _InterlockedExchange(long volatile *, long))
__MACHINEIA64(__int64 _InterlockedExchange64(__int64 volatile *, __int64))
__MACHINEIA64(__int64 _InterlockedExchange64_acq(__int64 volatile *, __int64))
__MACHINEX64(__int64 _InterlockedExchange64(__int64 volatile *, __int64))
__MACHINEIA64(void * _InterlockedExchangePointer(void * volatile *, void *))
__MACHINEIA64(void * _InterlockedExchangePointer_acq(void * volatile *, void *))
__MACHINEX64(void * _InterlockedExchangePointer(void * volatile *, void *))
__MACHINEI(long _InterlockedExchangeAdd(long volatile *, long))
__MACHINEIA64(long _InterlockedExchangeAdd(long volatile *, long))
__MACHINEIA64(long _InterlockedExchangeAdd_acq(long volatile *, long))
__MACHINEIA64(long _InterlockedExchangeAdd_rel(long volatile *, long))
__MACHINEIA64(__int64 _InterlockedExchangeAdd64(__int64 volatile *, __int64))
__MACHINEIA64(__int64 _InterlockedExchangeAdd64_acq(__int64 volatile *, __int64))
__MACHINEIA64(__int64 _InterlockedExchangeAdd64_rel(__int64 volatile *, __int64))
__MACHINEX64(__int64 _InterlockedExchangeAdd64(__int64 volatile *, __int64))
__MACHINEI(long _InterlockedCompareExchange (long volatile *, long, long))
__MACHINEIA64(long _InterlockedCompareExchange (long volatile *, long, long))
__MACHINEIA64(long _InterlockedCompareExchange_acq (long volatile *, long, long))
__MACHINEIA64(long _InterlockedCompareExchange_rel (long volatile *, long, long))
__MACHINEIA64(__int64 _InterlockedCompareExchange64(__int64 volatile *, __int64, __int64))
__MACHINEIA64(__int64 _InterlockedCompareExchange64_acq(__int64 volatile *, __int64, __int64))
__MACHINEIA64(__int64 _InterlockedCompareExchange64_rel(__int64 volatile *, __int64, __int64))
__MACHINEIA64(__int64 _InterlockedCompare64Exchange128(__int64 volatile *Destination, __int64 ExchangeHigh, __int64 ExchangeLow, __int64 Comparand))
__MACHINEIA64(__int64 _InterlockedCompare64Exchange128_acq(__int64 volatile *Destination, __int64 ExchangeHigh, __int64 ExchangeLow, __int64 Comparand))
__MACHINEIA64(__int64 _InterlockedCompare64Exchange128_rel(__int64 volatile *Destination, __int64 ExchangeHigh, __int64 ExchangeLow, __int64 Comparand))
__MACHINEIA64(void *_InterlockedCompareExchangePointer (void * volatile *, void *, void *))
__MACHINEIA64(void *_InterlockedCompareExchangePointer_acq (void * volatile *, void *, void *))
__MACHINEIA64(void *_InterlockedCompareExchangePointer_rel (void * volatile *, void *, void *))
__MACHINEI(__int64 _InterlockedCompareExchange64(__int64 volatile *, __int64, __int64))
__MACHINEX64(void *_InterlockedCompareExchangePointer (void * volatile *, void *, void *))
__MACHINEI(long __cdecl _InterlockedIncrement(long volatile *))
__MACHINEIA64(long _InterlockedIncrement(long volatile *))
__MACHINEIA64(long _InterlockedIncrement_acq(long volatile *))
__MACHINEIA64(long _InterlockedIncrement_rel(long volatile *))
__MACHINEIA64(__int64 _InterlockedIncrement64(__int64 volatile *))
__MACHINEIA64(__int64 _InterlockedIncrement64_acq(__int64 volatile *))
__MACHINEIA64(__int64 _InterlockedIncrement64_rel(__int64 volatile *))
__MACHINEX64(__int64 _InterlockedIncrement64(__int64 volatile *))
__MACHINEIW64(long _InterlockedOr(long volatile *, long))
__MACHINEIW64(char _InterlockedOr8(char volatile *, char))
__MACHINEIW64(short _InterlockedOr16(short volatile *, short))
__MACHINEW64(__int64 _InterlockedOr64(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedOr_acq(long volatile *, long))
__MACHINEIA64(char _InterlockedOr8_acq(char volatile *, char))
__MACHINEIA64(short _InterlockedOr16_acq(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedOr64_acq(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedOr_rel(long volatile *, long))
__MACHINEIA64(char _InterlockedOr8_rel(char volatile *, char))
__MACHINEIA64(short _InterlockedOr16_rel(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedOr64_rel(__int64 volatile *, __int64))
__MACHINEIW64(long _InterlockedXor(long volatile *, long))
__MACHINEIW64(char _InterlockedXor8(char volatile *, char))
__MACHINEIW64(short _InterlockedXor16(short volatile *, short))
__MACHINEW64(__int64 _InterlockedXor64(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedXor_acq(long volatile *, long))
__MACHINEIA64(char _InterlockedXor8_acq(char volatile *, char))
__MACHINEIA64(short _InterlockedXor16_acq(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedXor64_acq(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedXor_rel(long volatile *, long))
__MACHINEIA64(char _InterlockedXor8_rel(char volatile *, char))
__MACHINEIA64(short _InterlockedXor16_rel(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedXor64_rel(__int64 volatile *, __int64))
__MACHINEIW64(long _InterlockedAnd(long volatile *, long))
__MACHINEIW64(char _InterlockedAnd8(char volatile *, char))
__MACHINEIW64(short _InterlockedAnd16(short volatile *, short))
__MACHINEW64(__int64 _InterlockedAnd64(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedAnd_acq(long volatile *, long))
__MACHINEIA64(char _InterlockedAnd8_acq(char volatile *, char))
__MACHINEIA64(short _InterlockedAnd16_acq(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedAnd64_acq(__int64 volatile *, __int64))
__MACHINEIA64(long _InterlockedAnd_rel(long volatile *, long))
__MACHINEIA64(char _InterlockedAnd8_rel(char volatile *, char))
__MACHINEIA64(short _InterlockedAnd16_rel(short volatile *, short))
__MACHINEIA64(__int64 _InterlockedAnd64_rel(__int64 volatile *, __int64))
__MACHINEIA32(long _InterlockedAddLargeStatistic(__int64 volatile *, long))
__MACHINEI(int __cdecl _inp(unsigned short))
__MACHINEI(int __cdecl inp(unsigned short))
__MACHINEI(unsigned long __cdecl _inpd(unsigned short))
__MACHINEI(unsigned long __cdecl inpd(unsigned short))
__MACHINEI(unsigned short __cdecl _inpw(unsigned short))
__MACHINEI(unsigned short __cdecl inpw(unsigned short))
__MACHINEIA64(int __isNat(int))
__MACHINEIA64(void __isrlz(void))
__MACHINEIA64(void __invalat(void))

__MACHINE(long __cdecl labs(long))
__MACHINEIA64(void __lfetch(int, volatile void const *))
__MACHINEIA64(void __lfetchfault(int, volatile void const *))
__MACHINEIA64(void __lfetch_excl(int, volatile void const *))
__MACHINEIA64(void __lfetchfault_excl(int, volatile void const *))
__MACHINEIA64(__int64 __load128(void *, __int64 *))
__MACHINEIA64(__int64 __load128_acq(void *, __int64 *))

__MACHINE(unsigned long __cdecl _lrotl(unsigned long,int))
__MACHINE(unsigned long __cdecl _lrotr(unsigned long,int))
__MACHINEI(unsigned __int64  __ll_lshift(unsigned __int64,int))
__MACHINEI(__int64  __ll_rshift(__int64,int))

/*****
__MACHINE(int __cdecl memcmp(const void *,const void *,size_t _Size))
__MACHINE(void * __cdecl memcpy(void *,const void *,size_t _Size))
__MACHINE(void * __cdecl memset(void *,int,size_t _Size))
*/

__MACHINEIA64(void __mf(void))
__MACHINEIA64(void __mfa(void))

__MACHINEI(int __cdecl _outp(unsigned short,int))
__MACHINEI(int __cdecl outp(unsigned short,int))
__MACHINEI(unsigned long __cdecl _outpd(unsigned short,unsigned long))
__MACHINEI(unsigned long __cdecl outpd(unsigned short,unsigned long))
__MACHINEI(unsigned short __cdecl _outpw(unsigned short,unsigned short))
__MACHINEI(unsigned short __cdecl outpw(unsigned short,unsigned short))

__MACHINEIA64(void __ptcl(__int64, __int64))
__MACHINEIA64(void __ptcg(__int64, __int64))
__MACHINEIA64(void __ptcga(__int64, __int64))
__MACHINEIA64(void __ptrd(__int64, __int64))
__MACHINEIA64(void __ptri(__int64, __int64))
__MACHINEIA64(void *_rdteb(void))
__MACHINEIA64(void *_rdtebex(void))
__MACHINESA(int _ReadStatusReg(int))

__MACHINEIA64(void _ReleaseSpinLock(unsigned __int64 * ))
__MACHINEI(void * _ReturnAddress(void))
__MACHINEIA64(void * _ReturnAddress(void))
__MACHINESA(void * _ReturnAddress(void))

__MACHINE(unsigned int __cdecl _rotl(unsigned int,int))

__MACHINE(unsigned int __cdecl _rotr(unsigned int,int))

__MACHINEIA64(void __rsm(int))
__MACHINEIA64(void __rum(int))

__MACHINEIA64(void __setReg(int,unsigned __int64))

__MACHINEIA64(void __store128(void *, __int64, __int64))
__MACHINEIA64(void __store128_rel(void *, __int64, __int64))

/****
__MACHINE(char * __cdecl strcat(char *,const char * _Source))
__MACHINE(int __cdecl strcmp(const char *,const char *))
__MACHINE(char * __cdecl strcpy(char *,const char * _Source))
__MACHINE(size_t __cdecl strlen(const char *))
__MACHINE(char * __cdecl _strset(char *,int))
__MACHINE(char * __cdecl strset(char *,int))
*/

__MACHINEIA64(void __ssm(int))
__MACHINEIA64(void __sum(int))
__MACHINESA(int __swi(unsigned, ...))
__MACHINEIA64(void __synci(void))
__MACHINEIA64(__int64 __thash(__int64))
__MACHINEIA64(__int64 __ttag(__int64))

__MACHINEI(unsigned __int64 __ull_rshift(unsigned __int64,int))
__MACHINEIA64(unsigned __int64 __UMULH(unsigned __int64 a, unsigned __int64 b))

__MACHINESA(void _WriteStatusReg(int,int, int))
__MACHINEI(void * _AddressOfReturnAddress(void))
__MACHINEIA64(void __yield(void))
__MACHINEIA64(void __fci(void*))

__MACHINEI(void _WriteBarrier(void))
__MACHINEI(void _ReadWriteBarrier(void))
__MACHINEIA64(void _WriteBarrier(void))
__MACHINEIA64(void _ReadWriteBarrier(void))
__MACHINEX64(void __faststorefence(void))

__MACHINEI(void __halt(void))
__MACHINEI(void __sidt(void*))
__MACHINEI(void __lidt(void*))
__MACHINEI(void __ud2(void))
__MACHINEI(void __nop(void))

__MACHINEI(void __stosb(unsigned char *, unsigned char, size_t))
__MACHINEI(void __stosw(unsigned short *,  unsigned short, size_t))
__MACHINEI(void __stosd(unsigned long *,  unsigned long, size_t))
__MACHINEX64(void __stosq(unsigned __int64 *,  unsigned __int64, size_t))
__MACHINEIW64(unsigned char _bittest(long const *a, long b))
__MACHINEIW64(unsigned char _bittestandset(long *a, long b))
__MACHINEIW64(unsigned char _bittestandreset(long *a, long b))
__MACHINEIW64(unsigned char _bittestandcomplement(long *a, long b))
__MACHINEI(unsigned char _interlockedbittestandset(long volatile *a, long b))
__MACHINEI(unsigned char _interlockedbittestandreset(long volatile *a, long b))
__MACHINEW64(unsigned char _bittest64(__int64 const *a, __int64 b))
__MACHINEW64(unsigned char _bittestandset64(__int64 *a, __int64 b))
__MACHINEW64(unsigned char _bittestandreset64(__int64 *a, __int64 b))
__MACHINEW64(unsigned char _bittestandcomplement64(__int64 *a, __int64 b))
__MACHINEX64(unsigned char _interlockedbittestandset64(__int64 volatile *a, __int64 b))
__MACHINEX64(unsigned char _interlockedbittestandreset64(__int64 volatile *a, __int64 b))
__MACHINEI(void __cpuid(int a[4], int b))
__MACHINEI(void __cpuidex(int a[4], int b, int c))


__MACHINEIA32(unsigned char __readfsbyte(unsigned long Offset))
__MACHINEIA32(unsigned short __readfsword(unsigned long Offset))
__MACHINEIA32(unsigned long __readfsdword(unsigned long Offset))
__MACHINEIA32(unsigned __int64 __readfsqword(unsigned long Offset))
__MACHINEIA32(void __writefsbyte(unsigned long Offset, unsigned char Data))
__MACHINEIA32(void __writefsword(unsigned long Offset, unsigned short Data))
__MACHINEIA32(void __writefsdword(unsigned long Offset, unsigned long Data))
__MACHINEIA32(void __writefsqword(unsigned long Offset, unsigned __int64 Data))


__MACHINE(unsigned __int64 __cdecl _rotl64(unsigned __int64,int))
__MACHINE(unsigned __int64 __cdecl _rotr64(unsigned __int64,int))
__MACHINE(__int64 __cdecl _abs64(__int64))

__MACHINEIW64(unsigned char _BitScanForward(unsigned long* Index, unsigned long Mask))
__MACHINEIW64(unsigned char _BitScanReverse(unsigned long* Index, unsigned long Mask))
__MACHINEW64(unsigned char _BitScanForward64(unsigned long* Index, unsigned __int64 Mask))
__MACHINEW64(unsigned char _BitScanReverse64(unsigned long* Index, unsigned __int64 Mask))
__MACHINEIW64(_CRTIMP wchar_t * __cdecl wcscat(wchar_t *, const wchar_t * _Source))
__MACHINEIW64(_CRTIMP int __cdecl wcscmp(const wchar_t *, const wchar_t *))
__MACHINEIW64(_CRTIMP wchar_t * __cdecl wcscpy(wchar_t *, const wchar_t * _Source))
__MACHINEIW64(_CRTIMP size_t __cdecl wcslen(const wchar_t *))
#pragma warning(suppress: 4985)
__MACHINEIW64(_CRTIMP wchar_t * __cdecl _wcsset(wchar_t *, wchar_t))
__MACHINEW64(unsigned __int64 __shiftleft128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift))
__MACHINEW64(unsigned __int64 __shiftright128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift))

__MACHINEI(void __int2c(void))
__MACHINEIW64(void _ReadBarrier(void))
__MACHINEIW64(unsigned char _rotr8(unsigned char value, unsigned char shift))
__MACHINEIW64(unsigned short _rotr16(unsigned short value, unsigned char shift))
__MACHINEIW64(unsigned char _rotl8(unsigned char value, unsigned char shift))
__MACHINEIW64(unsigned short _rotl16(unsigned short value, unsigned char shift))
__MACHINEIW64(short _InterlockedIncrement16(short volatile *Addend))
__MACHINEIW64(short _InterlockedDecrement16(short volatile *Addend))
__MACHINEIW64(short _InterlockedCompareExchange16(short volatile *Destination, short Exchange, short Comparand))
__MACHINEIA64(short _InterlockedIncrement16_acq(short volatile *Addend))
__MACHINEIA64(short _InterlockedIncrement16_rel(short volatile *Addend))
__MACHINEIA64(short _InterlockedDecrement16_acq(short volatile *Addend))
__MACHINEIA64(short _InterlockedDecrement16_rel(short volatile *Addend))
__MACHINEIA64(short _InterlockedCompareExchange16_acq(short volatile *Destination, short Exchange, short Comparand))
__MACHINEIA64(short _InterlockedCompareExchange16_rel(short volatile *Destination, short Exchange, short Comparand))


__MACHINEX64(short _InterlockedCompareExchange16_np(short volatile *Destination, short Exchange, short Comparand))
__MACHINEX64(long _InterlockedCompareExchange_np (long *, long, long))
__MACHINEX64(__int64 _InterlockedCompareExchange64_np(__int64 *, __int64, __int64))
__MACHINEX64(void *_InterlockedCompareExchangePointer_np (void **, void *, void *))
__MACHINEX64(unsigned char _InterlockedCompareExchange128(__int64 volatile *, __int64, __int64, __int64 *))
__MACHINEX64(unsigned char _InterlockedCompareExchange128_np(__int64 volatile *, __int64, __int64, __int64 *))
__MACHINEX64(long _InterlockedAnd_np(long *, long))
__MACHINEX64(char _InterlockedAnd8_np(char *, char))
__MACHINEX64(short _InterlockedAnd16_np(short *, short))
__MACHINEX64(__int64 _InterlockedAnd64_np(__int64 *, __int64))
__MACHINEX64(long _InterlockedOr_np(long *, long))
__MACHINEX64(char _InterlockedOr8_np(char *, char))
__MACHINEX64(short _InterlockedOr16_np(short *, short))
__MACHINEX64(__int64 _InterlockedOr64_np(__int64 *, __int64))
__MACHINEX64(long _InterlockedXor_np(long *, long))
__MACHINEX64(char _InterlockedXor8_np(char *, char))
__MACHINEX64(short _InterlockedXor16_np(short *, short))
__MACHINEX64(__int64 _InterlockedXor64_np(__int64 *, __int64))

__MACHINEI(unsigned short __lzcnt16(unsigned short))
__MACHINEI(unsigned int __lzcnt(unsigned int))
__MACHINEX64(unsigned __int64 __lzcnt64(unsigned __int64))
__MACHINEI(unsigned short __popcnt16(unsigned short))
__MACHINEI(unsigned int __popcnt(unsigned int))
__MACHINEX64(unsigned __int64 __popcnt64(unsigned __int64))

__MACHINEI(unsigned __int64 __rdtscp(unsigned int*))

__MACHINEX86X_X64(void _mm_pause(void))

__END_DECLS

#endif /** __QKC_WSTUB_INTRIN_H */
