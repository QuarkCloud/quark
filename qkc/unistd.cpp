
#include <unistd.h>
#include <wintf/wcrt.h>

off_t lseek(int fd , off_t offset , int whence)
{
    return ::_lseek(fd , offset , whence) ;
}

off64_t lseek64(int fd , off64_t offset , int whence)
{
    return ::_lseeki64(fd , offset , whence) ;
}

int close(int fd)
{
    return ::_close(fd) ;
}

ssize_t read(int fd , void * buf , size_t nbytes)
{
    return ::_read(fd , buf , nbytes) ;
}

ssize_t write(int fd , const void * buf , size_t n)
{
    return ::_write(fd , buf , n) ;
}

unsigned int sleep(unsigned int seconds)
{
    ::_sleep(seconds) ;
    return 0 ;
}

int usleep(useconds_t useconds)
{
    return 0 ;
}

int pause()
{
    return 0 ;
}

int chown(const char * file , uid_t owner , gid_t group)
{
    return 0 ;
}

int chdir(const char * path)
{
    return ::_chdir(path) ;
}

char *getcwd(char * buf , size_t size)
{
    return ::_getcwd(buf , size) ;
}

int dup(int fd)
{
    return ::_dup(fd) ;
}

int dup2(int fd1 , int fd2)
{
    return ::_dup2(fd1 , fd2) ;
}

long int sysconf(int name)
{
    return 0 ;
}

pid_t getpid(void)
{
    return ::_getpid() ;
}

pid_t fork(void)
{
    return 0 ;
}

int rmdir(const char * path)
{
    return ::_rmdir(path) ;
}

int gethostname(char * name , size_t len)
{
    return 0 ;
}

int sethostname(const char * name , size_t len)
{
    return 0 ;
}

int daemon(int nochdir , int noclose)
{
    return 0 ;
}

int fsync(int fd)
{
    return 0 ;
}

void sync(void)
{
    //
}

int getpagesize(void)
{
    return 0 ;
}

int truncate(const char * file , off_t length)
{
    return 0 ;
}

int truncate64(const char * file , off64_t length)
{
    return 0 ;
}

int ftruncate(int fd , off_t length) 
{
    return ::_chsize(fd , length) ;
}

int ftruncate64(int fd , off64_t length) 
{
    return ::_chsize_s(fd , length) ;
}

int brk(void * addr)
{
    return 0 ;
}

void *sbrk(intptr_t delta) 
{
    return NULL ;
}

long int syscall(long int sysno , ...) 
{
    return 0 ;
}

int lockf(int fd , int cmd , off_t len)
{
    return _locking(fd , cmd , len) ;
}

int lockf64(int fd , int cmd , off64_t len)
{
    return _locking(fd , cmd , (off_t)len) ;
}

char *crypt(const char *key , const char *salt) 
{
    return NULL ;
}

void encrypt(char *block , int edflag)
{
    //
}

