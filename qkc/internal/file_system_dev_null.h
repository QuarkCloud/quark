
#ifndef __QKC_INTERNAL_FILE_SYSTEM_DEV_NULL_H
#define __QKC_INTERNAL_FILE_SYSTEM_DEV_NULL_H 1

#include <windows.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef	__cplusplus
extern "C" {
#endif

static const char * file_system_dev_null_name = "/dev/null" ;

int file_system_dev_null_creat(const char * file , mode_t mode) ;
int file_system_dev_null_open(const char * path , int oflag , ...) ;
void file_system_dev_null_close(int fd) ;
int file_system_dev_null_fcntl(int fd , int cmd , ...) ;

ssize_t file_system_dev_null_write(int fd , const void * buf , size_t bytes) ;
ssize_t file_system_dev_null_read(int fd , void * buf , size_t bytes) ;
int file_system_dev_null_dup(int fd) ;
int file_system_dev_null_dup2(int fd1 , int fd2) ;
int file_system_dev_null_lseek(int fd , off_t offset , int whence) ;
int file_system_dev_null_fstat(int fd , struct stat * stat) ;

void file_system_dev_null_init() ;
void file_system_dev_null_final() ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_FILE_SYSTEM_DEV_NULL_H */
