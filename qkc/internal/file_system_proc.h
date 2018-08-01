
#ifndef __QKC_INTERNAL_FILE_SYSTEM_PROC_H
#define __QKC_INTERNAL_FILE_SYSTEM_PROC_H 1

#include <quark_compile.h>
#include <windows.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

__BEGIN_DECLS

static const char * file_system_proc_name = "/proc" ;

int file_system_proc_creat(const char * file , mode_t mode) ;
int file_system_proc_open(const char * path , int oflag , ...) ;
void file_system_proc_close(int fd) ;
int file_system_proc_fcntl(int fd , int cmd , ...) ;

ssize_t file_system_proc_write(int fd , const void * buf , size_t bytes) ;
ssize_t file_system_proc_read(int fd , void * buf , size_t bytes) ;
int file_system_proc_dup(int fd) ;
int file_system_proc_dup2(int fd1 , int fd2) ;
int file_system_proc_lseek(int fd , off_t offset , int whence) ;
int file_system_proc_fstat(int fd , struct stat * stat) ;

void file_system_proc_init() ;
void file_system_proc_final() ;

__END_DECLS

#endif /** __QKC_INTERNAL_FILE_SYSTEM_PROC_H */
