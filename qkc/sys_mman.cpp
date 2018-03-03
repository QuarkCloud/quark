
#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>

void *mmap (void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    /**
        fd是linux系统的句柄，需要转译成windows的句柄HANDLE
    */
    HANDLE handle = (HANDLE)::_get_osfhandle(fd) ;
    return NULL ;
}

int munmap (void *addr, size_t len)
{
    return 0 ;
}

int mprotect (void *addr, size_t len, int prot)
{
    return 0 ;
}

int msync (void *addr, size_t len, int flags)
{
    return 0 ;
}

int mlock (const void *addr, size_t len)
{
    return 0 ;
}

int munlock (const void *addr, size_t len)
{
    return 0 ;
}

int mlockall (int flags)
{
    return 0 ;
}

int munlockall (void)
{
    return 0 ;
}

int shm_open (const char *name, int oflag , mode_t mode)
{
    return ::_open(name , oflag , mode) ;
}

int shm_unlink (const char *name)
{
    return ::_unlink(name) ;
}
