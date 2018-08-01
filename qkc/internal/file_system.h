
#ifndef __QKC_INTERNAL_FILE_SYSTEM_H
#define __QKC_INTERNAL_FILE_SYSTEM_H 1

#include <windows.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

__BEGIN_DECLS

//定义访问接口
typedef struct __st_file_system{
    int (*creat)(const char * file , mode_t mode) ;
    int (*open)(const char * path , int oflag , ...) ;
    void (*close)(int fd) ;
    int (*fcntl)(int fd , int cmd , ...) ;

    ssize_t (*write)(int fd , const void * buf , size_t bytes) ;
    ssize_t (*read)(int fd , void * buf , size_t bytes) ;
    int (*dup)(int fd) ;
    int (*dup2)(int fd1 , int fd2) ;
    int (*lseek)(int fd , off_t offset , int whence) ;
    int (*fstat)(int fd , struct stat * stat) ;

    void * data ;
} file_system_t ;

int file_system_mount(const char * name , file_system_t * fs) ;

int file_system_unmount(const char * name) ;

file_system_t * file_system_find(const char * name) ;

__END_DECLS

#endif /** __QKC_INTERNAL_FILE_SYSTEM_H */
