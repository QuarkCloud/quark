
#include <syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

long int syscall(long int sysno , ...) 
{
    if(sysno == __NR_pipe2)
    {
        va_list ap;
        int *fds , flags = 0 ;
        va_start (ap, sysno);
        fds = va_arg (ap, int *);
        flags = va_arg (ap, int);
        va_end (ap);

        return pipe2(fds , flags) ;
    }
    else
    {
        errno = ENOSYS ;
        return -1 ;
    }
}

