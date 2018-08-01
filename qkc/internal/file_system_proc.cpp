
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wintf/wobj.h"
#include "file_system.h"
#include "file_system_proc.h"


static file_system_t  file_system_proc ;

typedef struct{
    char * path ;
} fs_proc_info_t ;

static int file_system_proc_pid_stat_read(pid_t pid , void * buf , size_t size) ;

int file_system_proc_creat(const char * file , mode_t mode)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_open(const char * path , int oflag , ...)
{
    size_t isize = sizeof(fs_proc_info_t) ;
    fs_proc_info_t * info = (fs_proc_info_t *)::malloc(isize) ;
    info->path = ::strdup(path) ;

    return ::wobj_set(WOBJ_FILE , info , &file_system_proc) ;
}

void file_system_proc_close(int fd)
{
    wobj_del(fd) ;
}

int file_system_proc_fcntl(int fd , int cmd , ...)
{
    errno = ENOSYS ;
    return -1 ;
}

ssize_t file_system_proc_write(int fd , const void * buf , size_t bytes)
{
    errno = ENOSYS ;
    return -1 ;
}

ssize_t file_system_proc_read(int fd , void * buf , size_t bytes)
{
    wobj_t * obj = ::wobj_get(fd) ;
    if(obj == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }

    if(obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_dup(int fd)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_dup2(int fd1 , int fd2)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_lseek(int fd , off_t offset , int whence) 
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_fstat(int fd , struct stat * stat)
{
    errno = ENOSYS ;
    return -1 ;
}

void file_system_proc_init()
{
    file_system_proc.creat = file_system_proc_creat ;
    file_system_proc.open =  file_system_proc_open ;
    file_system_proc.close =  file_system_proc_close ;
    file_system_proc.fcntl =  file_system_proc_fcntl ;
    file_system_proc.write =  file_system_proc_write ;
    file_system_proc.read =  file_system_proc_read ;

    file_system_proc.dup =  file_system_proc_dup ;
    file_system_proc.dup2 =  file_system_proc_dup2 ;
    file_system_proc.lseek =  file_system_proc_lseek ;
    file_system_proc.fstat =  file_system_proc_fstat ;
    file_system_proc.data =  NULL;

    file_system_mount(file_system_proc_name , &file_system_proc) ;
}

void file_system_proc_final()
{
    file_system_unmount(file_system_proc_name) ;
}

static int file_system_proc_pid_stat_read(pid_t pid , void * buf , size_t size) 
{
    return 0 ;
}

