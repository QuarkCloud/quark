
#ifndef __QUARK_WSTUB_USERENV_H
#define __QUARK_WSTUB_USERENV_H 1

#include <quark_compile.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (WINAPI *LPFN_GETUSERPROFILEDIRECTORW)(HANDLE token ,LPWSTR profile,LPDWORD size);
int _imp_get_user_directory(char * dir , int size) ;

#ifdef __cplusplus
}
#endif

#endif /** __QUARK_WSTUB_USERENV_H */
