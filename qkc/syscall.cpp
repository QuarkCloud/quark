
#include <syscall.h>
#include <unistd.h>
#include <errno.h>

long int syscall(long int sysno , ...) 
{
    if(sysno == __NR_pipe2)
    {
        return -1 ;
    }
    else
    {
        errno = ENOSYS ;
        return -1 ;
    }
}

