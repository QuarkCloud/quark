
#include <unistd.h>
#include <errno.h>
#include <wintf/wcrt.h>
#include <wintf/wobj.h>
#include <winsock2.h>
#include "internal/inotify_mgr.h"
#include "internal/sysconf.h"
#include "internal/intrin.h"
#include "internal/file_system.h"


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
    if(fd < IOINFO_ARRAYS)
        return ::_close(fd) ;

    wobj_t * wobj = wobj_get(fd) ;
    if(wobj == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }

    int result = 0 ;
    wobj_type type = wobj->type ;
    if(type == WOBJ_OTHR || type == WOBJ_PROC || type == WOBJ_THRD || 
        type == WOBJ_MUTEX || type == WOBJ_SEMA || type == WOBJ_EVENT || 
        type == WOBJ_IOCP)
    {
        ::CloseHandle(wobj->handle) ;
    }
    else if(type == WOBJ_MODU)
    {
        ::FreeLibrary((HMODULE)wobj->handle) ;
    }
    else if(type == WOBJ_SOCK)
    {
        ::_imp_closesocket((SOCKET)wobj->handle) ;
    }
    else if(type == WOBJ_NOTF) 
    {
        inotify_mgr_free((inotify_mgr_t *)wobj->addition) ;
    }
    else if(type == WOBJ_FILE)
    {
        file_system_t * fs = (file_system_t *)wobj->addition ;
        if(fs != NULL)
        {
            fs->close(fd) ;
        }
    }
    else if(type == WOBJ_PIPE)
    {
        return ::_close((int)wobj->handle) ;
    }
    else
    {
        errno = ENOSYS ;
        result = -1 ;
    }

    ::wobj_del(fd) ;
    return result ;
}

ssize_t read(int fd , void * buf , size_t nbytes)
{
    if(fd < IOINFO_ARRAYS)
        return ::_read(fd , buf , nbytes) ;

    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL)
        return -1 ;
    if(obj->type == WOBJ_PIPE)
        return ::_read((int)obj->handle , buf , nbytes) ;

    if(obj->type != WOBJ_FILE || obj->addition == NULL)
        return -1 ;

    file_system_t * file_system = (file_system_t *)obj->addition ;
    
    return file_system->read(fd , buf , nbytes) ;
}

ssize_t write(int fd , const void * buf , size_t nbytes)
{
    if(fd < IOINFO_ARRAYS)
        return ::_write(fd , buf , nbytes) ;

    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL)
        return -1 ;
    if(obj->type == WOBJ_PIPE)
        return ::_write((int)obj->handle , buf , nbytes) ;

    if(obj->type != WOBJ_FILE || obj->addition == NULL)
        return -1 ;

    file_system_t * file_system = (file_system_t *)obj->addition ;
    
    return file_system->write(fd , buf , nbytes) ;
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

#pragma intrinsic(_mm_pause)
int pause()
{
    _mm_pause() ;
    return 0 ;
}

#ifndef O_BINARY
#define O_BINARY 0x8000
#endif

#define PIPE_SIZE 4096

void add_pipe_objs(int pipedes[2] , int oids[2])
{
    oids[0] = ::wobj_set(WOBJ_PIPE , (HANDLE)pipedes[0] , NULL) ;
    oids[1] = ::wobj_set(WOBJ_PIPE , (HANDLE)pipedes[1] , NULL) ;
}

int pipe (int pipedes[2])
{
    int result = 0 ;
    int fds[2] = {0 , 0} ;
    if((result = _pipe(fds , PIPE_SIZE , O_BINARY)) == 0)
        add_pipe_objs(fds , pipedes) ;

    return result ;
}

int pipe2(int pipedes[2] , int flags) 
{
    int result = 0 ;
    int fds[2] = {0 , 0} ;
    if((result = _pipe(fds , PIPE_SIZE , O_BINARY)) == 0)
        add_pipe_objs(fds , pipedes) ;
    return result ;
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
    if(name == _SC_PAGESIZE)
        return __get_pagesize() ;
    else if(name == _SC_NPROCESSORS_ONLN)
        return __get_num_of_processor() ;

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
    HANDLE handle = (HANDLE)::_get_osfhandle(fd) ;
    if(handle == NULL || handle == INVALID_HANDLE_VALUE)
    {
        errno = EINVAL ;
        return -1 ;
    }

    if(::FlushFileBuffers(handle) == TRUE)
        return 0 ;
    else
        return -1 ;
}

void sync(void)
{
    for(int fd = 3 ; fd < 256 ; ++fd)
    {
        HANDLE handle = (HANDLE)::_get_osfhandle(fd) ;
        if(handle == NULL || handle == INVALID_HANDLE_VALUE)
            continue ;

        ::FlushFileBuffers(handle) ;    
    }
}

int getpagesize(void)
{
    SYSTEM_INFO info ;
    ::GetSystemInfo(&info) ;
    return (int)info.dwPageSize ;
}

int truncate(const char * file , off_t length)
{
    int fd = ::_open(file , 0 , 0) ;
    int result = ftruncate(fd , length) ;
    ::_close(fd) ;
    return result ;
}

int truncate64(const char * file , off64_t length)
{
    int fd = ::_open(file , 0 , 0) ;
    int result = ftruncate64(fd , length) ;
    ::_close(fd) ;
    return result ;
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
/**
long int syscall(long int sysno , ...) 
{
    return 0 ;
}
*/
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


uid_t getuid ()
{
    return 0 ;
}

uid_t geteuid ()
{
    return 0 ;
}

gid_t getgid ()
{
    return 0 ;
}

gid_t getegid ()
{
    return 0 ;
}

int setuid (uid_t uid)
{
    return 0 ;
}

int setreuid (uid_t ruid, uid_t euid)
{
    return 0 ;
}

int seteuid (uid_t uid)
{
    return 0 ;
}

int setgid (gid_t gid)
{
    return 0 ;
}

int setegid (gid_t gid)
{
    return 0 ;
}

pid_t setsid (void)
{
    return 0 ;
}

ssize_t pread (int fd, void *buf, size_t nbytes, off_t offset) 
{
    return 0 ;
}

ssize_t pwrite (int fd, const void *buf, size_t n,off_t offset)
{
    return 0 ;
}

long int pathconf (const char *path, int name)
{
    return 0 ;
}

long int fpathconf (int fd, int name)
{
    return 0 ;
}

int symlink (const char * from, const char * to)
{
    return -1 ;
}

ssize_t readlink (const char *path, char *buf, size_t len)
{
    return -1 ;
}

int link (const char * from, const char * to)
{
    return -1 ;
}

int fchown (int fd, uid_t owner, gid_t group)
{
    return -1 ;
}

char *ttyname (int fd)
{
    return NULL ;
}

int ttyname_r (int fd, char *buf, size_t buflen)
{
    return -1 ;
}

 int isatty (int fd) 
 {
    return -1 ;
 }

