
#include <userenv.h>
#include <windows.h>

typedef struct __st_lpfn_userenv{
    LPFN_GETUSERPROFILEDIRECTORW    lpfn_GetUserProfileDirectoryW ;
} lpfn_userenv_t ;

SRWLOCK __userenv_internal_rwlock__ =  SRWLOCK_INIT ;
static bool __userenv_internal_rwlock_inited__ = false ;
static lpfn_userenv_t __userenv_pfns__ ;

#define DECLARE_USERENV_PFN(name , type) __userenv_pfns__.lpfn_##name = (type)::GetProcAddress(module , #name)

bool userenv_library_load()
{
    HMODULE module = ::LoadLibrary("userenv.dll") ;
    if(module == NULL)
        return false ;
    
    DECLARE_USERENV_PFN(GetUserProfileDirectoryW ,           LPFN_GETUSERPROFILEDIRECTORW);

    return  true ;
}

bool userenv_library_init()
{
    if(__userenv_internal_rwlock_inited__ == false)
    {
        ::AcquireSRWLockExclusive(&__userenv_internal_rwlock__) ;

        if(__userenv_internal_rwlock_inited__ == false)
            __userenv_internal_rwlock_inited__ = userenv_library_load() ;
        ::ReleaseSRWLockExclusive(&__userenv_internal_rwlock__) ;
    }

    return __userenv_internal_rwlock_inited__ ;
}

int _imp_get_user_directory(char * dir , int size)
{
    if(userenv_library_init() == false)
        return -1 ;

    HANDLE token = NULL ;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &token) == 0)
        return -1 ;

    WCHAR profile[256] ;
    DWORD profile_size = 256 ;
    BOOL is_got = __userenv_pfns__.lpfn_GetUserProfileDirectoryW(::GetCurrentProcess() , profile , &profile_size) ;
    ::CloseHandle(token) ;
    if(is_got == FALSE)
        return -1 ;

    int result = ::WideCharToMultiByte(CP_UTF8  , 0 , profile , profile_size , dir , size , NULL , NULL)  ;
    if(result >= 0)
        dir[result] = '\0' ;
    return result ;
}


