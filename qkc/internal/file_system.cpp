
#include "file_system.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

typedef struct __st_file_system_node{
    char * name ;
    size_t nsize ;
    file_system_t * file_system ;
} file_system_node_t ;

static SRWLOCK __file_system_locker__ = SRWLOCK_INIT ;
static const size_t kFileSystemMaxSize = 64 ;
static bool __file_systems_inited__ = false ;
static size_t __file_systems_size__ = 0 ;
static file_system_node_t __file_systems__[kFileSystemMaxSize] ;

int file_system_mount(const char * name , file_system_t * fs)
{
    if(name == NULL || fs == NULL)
        return -1 ;
    
    int result = -1 ;

    ::AcquireSRWLockExclusive(&__file_system_locker__) ;

    if(__file_systems_inited__ == false)
    {
        ::memset(__file_systems__ , 0 , sizeof(__file_systems__)) ;
        __file_systems_inited__ = true ;
    }

    char * str = NULL ;
    file_system_t * file_system = NULL ;

    size_t nsize = ::strlen(name) + 1 ;
    str = (char *)::malloc(nsize) ;
    ::memcpy(str , name , nsize) ;

    size_t fsize = sizeof(file_system_t) ;
    file_system = (file_system_t *)::malloc(fsize) ;
    ::memcpy(file_system , fs , fsize) ;

    file_system_node_t * node = __file_systems__ + __file_systems_size__ ;
    node->name = str ;
    node->nsize = nsize ;
    node->file_system = file_system ;
    ++__file_systems_size__ ;

    result = 0 ;

    ::ReleaseSRWLockExclusive(&__file_system_locker__) ;
    return result ;
}

int file_system_unmount(const char * name)
{
    if(name == NULL)
        return -1 ;

    int result = -1 ;
    ::AcquireSRWLockExclusive(&__file_system_locker__) ;

    for(size_t fidx = 0 ; fidx < __file_systems_size__ ; ++fidx)
    {
        file_system_node_t * node = __file_systems__ + fidx ;

        if(::strcmp(node->name , name) == 0)
        {
            //ÕÒµ½
            ::free(node->name) ;
            node->name = NULL ;
            ::free(node->file_system) ;
            node->file_system = NULL ;

            result = 0 ;
            break ;
        }    
    }

    ::ReleaseSRWLockExclusive(&__file_system_locker__) ;
    return result ;
}

file_system_t * file_system_find(const char * name) 
{
    if(name == NULL)
        return NULL ;
    size_t nsize = ::strlen(name) + 1;

    file_system_t * file_system = NULL;
    ::AcquireSRWLockShared(&__file_system_locker__) ;

    for(size_t fidx = 0 ; fidx < __file_systems_size__ ; ++fidx)
    {
        file_system_node_t * node = __file_systems__ + fidx ;
        if(node->nsize != nsize)
            continue ;

        if(::memcmp(node->name , name , nsize) == 0)
        {
            file_system = node->file_system ;
            break ;
        }    
    }

    ::ReleaseSRWLockShared(&__file_system_locker__) ;

    return file_system ;
}

