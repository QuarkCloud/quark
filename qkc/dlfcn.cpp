
#include <dlfcn.h>
#include <windows.h>
#include <wintf/wthr.h>

void *dlopen (const char * file, int mode)
{
    return ::LoadLibrary(file) ;
}

int dlclose (void * handle)
{
    if(::FreeLibrary((HMODULE)handle) == TRUE)
        return 0 ;
    else
        return -1 ;
}

void *dlsym_default(const char * name)
{
    HMODULE handle = (HMODULE)dll_handle_get() ;
    if(handle == NULL)
        return NULL ;

    return ::GetProcAddress(handle , name) ;
}

void *dlsym_special(void * handle , const char * name)
{
    return ::GetProcAddress((HMODULE)handle , name) ;
}

void *dlsym (void * handle , const char * name)
{
    if(handle == RTLD_DEFAULT || handle == RTLD_NEXT)
        return dlsym_default(name) ;
    else
        return dlsym_special(handle , name) ;
}

char *dlerror (void) 
{
    static char __dlerror__[] = "dlfcn failed" ;
    if(GetLastError() != 0)
        return __dlerror__ ;
    else
        return NULL ;
}


