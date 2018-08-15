
#ifndef __QUARK_WSTUB_ADVAPI32_H
#define __QUARK_WSTUB_ADVAPI32_H 1

#include <quark_compile.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (WINAPI *LPFN_GETUSERNAMEW)(LPWSTR buffer ,LPDWORD size);
int _imp_get_username(char * name , int size) ;


#ifdef __cplusplus
}
#endif

#endif /** __QUARK_WSTUB_ADVAPI32_H */
