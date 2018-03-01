
#include <dlfcn.h>
#include <windows.h>

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

void *dlsym (void * handle , const char * name)
{
    return ::GetProcAddress((HMODULE)handle , name) ;
}

char *dlerror (void) 
{
    static char __dlerror__[] = "dlfcn failed" ;
    if(GetLastError() != 0)
        return __dlerror__ ;
    else
        return NULL ;
}


