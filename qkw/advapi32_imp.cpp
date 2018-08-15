
#include <advapi32.h>
#include <windows.h>

typedef struct __st_lpfn_advapi32{
    LPFN_GETUSERNAMEW    lpfn_GetUserNameW ;
} lpfn_advapi32_t ;

SRWLOCK __advapi32_internal_rwlock__ =  SRWLOCK_INIT ;
static bool __advapi32_internal_rwlock_inited__ = false ;
static lpfn_advapi32_t __advapi32_pfns__ ;

#define DECLARE_ADVAPI32_PFN(name , type) __advapi32_pfns__.lpfn_##name = (type)::GetProcAddress(module , #name)

bool advapi32_library_load()
{
    HMODULE module = ::LoadLibrary("advapi32.dll") ;
    if(module == NULL)
        return false ;
    
    DECLARE_ADVAPI32_PFN(GetUserNameW ,           LPFN_GETUSERNAMEW);

    return  true ;
}

bool advapi32_library_init()
{
    if(__advapi32_internal_rwlock_inited__ == false)
    {
        ::AcquireSRWLockExclusive(&__advapi32_internal_rwlock__) ;

        if(__advapi32_internal_rwlock_inited__ == false)
            __advapi32_internal_rwlock_inited__ = advapi32_library_load() ;
        ::ReleaseSRWLockExclusive(&__advapi32_internal_rwlock__) ;
    }

    return __advapi32_internal_rwlock_inited__ ;
}

int _imp_get_username(char * name , int size) 
{
    if(advapi32_library_init() == false)
        return -1 ;

    WCHAR username[256] ;
    DWORD username_size = 256 ;
    if(__advapi32_pfns__.lpfn_GetUserNameW(username , &username_size) == FALSE)
        return -1 ;

    int result = ::WideCharToMultiByte(CP_UTF8  , 0 , username , username_size , name , size , NULL , NULL)  ;
    if(result >= 0)
        name[result] = '\0' ;
    return result ;
}

