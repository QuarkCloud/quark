
#include <sys/mman.h>
#include <windows.h>

void *mmap (void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
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
