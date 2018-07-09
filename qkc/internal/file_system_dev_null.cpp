
#include <windows.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "file_system_dev_null.h"
#include "wintf/wobj.h"

static file_system_t  file_system_dev_null ;

int file_system_dev_null_creat(const char * file , mode_t mode)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_dev_null_open(const char * path , int oflag , ...)
{
    if(path == NULL || ::strcmp(file_system_dev_null_name , path) != 0)
    {
        errno = EINVAL ;
        return -1 ;
    }

    return ::wobj_set(WOBJ_FILE , NULL , &file_system_dev_null) ;
}

void file_system_dev_null_close(int fd)
{
    wobj_del(fd) ;
}

int file_system_dev_null_fcntl(int fd , int cmd , ...)
{
    errno = ENOSYS ;
    return -1 ;
}

ssize_t file_system_dev_null_write(int fd , const void * buf , size_t bytes)
{
    return bytes ;
}

ssize_t file_system_dev_null_read(int fd , void * buf , size_t bytes)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_dev_null_dup(int fd)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_dev_null_dup2(int fd1 , int fd2)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_dev_null_lseek(int fd , off_t offset , int whence)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_dev_null_fstat(int fd , struct stat * stat)
{
    errno = ENOSYS ;
    return -1 ;
}

void file_system_dev_null_init()
{
    file_system_dev_null.creat = file_system_dev_null_creat ;
    file_system_dev_null.open =  file_system_dev_null_open ;
    file_system_dev_null.close =  file_system_dev_null_close ;
    file_system_dev_null.fcntl =  file_system_dev_null_fcntl ;
    file_system_dev_null.write =  file_system_dev_null_write ;
    file_system_dev_null.read =  file_system_dev_null_read ;

    file_system_dev_null.dup =  file_system_dev_null_dup ;
    file_system_dev_null.dup2 =  file_system_dev_null_dup2 ;
    file_system_dev_null.lseek =  file_system_dev_null_lseek ;
    file_system_dev_null.fstat =  file_system_dev_null_fstat ;
    file_system_dev_null.data =  NULL;

    file_system_mount(file_system_dev_null_name , &file_system_dev_null) ;
}

void file_system_dev_null_final()
{
    file_system_unmount(file_system_dev_null_name) ;
}

