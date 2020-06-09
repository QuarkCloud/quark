
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wobjs/ErrorMap.h"
#include "wobjs/CrtLinker.h"

char *** __f_environ()
{
    return &_environ;
}

int rand_r (unsigned int * seed)
{
    unsigned int value = 0 ;
    rand_s(&value) ;
    return (int)value ;
}

char * secure_getenv (const char *name) 
{
    //TO-DO
    return NULL ;
}

int putenv(char *str)
{
    return _putenv(str) ;
}

int setenv(const char *envname, const char *envval, int overwrite)
{
    return _putenv_s(envname , envval) ;
}

char *mkdtemp (char * template_str) 
{
    return NULL ;
}

char *realpath (const char * name, char *resolved)
{
    HANDLE handle = CreateFileA(name , 0, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (handle == INVALID_HANDLE_VALUE)
    {
        errno = oserr_map(::GetLastError()) ;
        return NULL;
    }

    char path[1024] = {'\0'} ;
    DWORD path_size = GetFinalPathNameByHandle(handle , path , 1024 , VOLUME_NAME_DOS) ;
    ::CloseHandle(handle) ;

    if(path_size == 0)
        return NULL ;

    path_size -= 4 ;

    char * result = resolved ;
    if(result == NULL)
        result = (char *)::malloc(path_size) ;

    ::memcpy(result , path + 4 , path_size) ;
    result[path_size] = '\0' ;
    return result ;
}

