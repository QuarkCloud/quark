
#ifndef __QUARK_WSTUB_WINSOCK2_H
#define __QUARK_WSTUB_WINSOCK2_H 1

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

ULONG _imp_if_nametoindex(PCSTR name) ;
typedef ULONG (WINAPI *LPFN_IF_NAMETOINDEX)(PCSTR InterfaceName) ;

PCHAR _imp_if_indextoname(ULONG index , PCHAR name) ;
typedef PCHAR (WINAPI *LPFN_IF_INDEXTONAME)(ULONG InterfaceIndex , PCHAR InterfaceName) ;

#ifdef __cplusplus
}
#endif

#endif  /** __QUARK_WSTUB_WINSOCK2_H */
