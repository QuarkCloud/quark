
#include <syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/epoll.h>
#include <sys/inotify.h>

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
    else if(sysno == __NR_epoll_create)
    {
        va_list ap ;
        int size = 0 ;
        va_start (ap, sysno);
        size = va_arg (ap, int);
        va_end (ap);

        return epoll_create(size) ;
    }
    else if(sysno == __NR_epoll_create1)
    {
        va_list ap ;
        int flags = 0 ;
        va_start (ap, sysno);
        flags = va_arg (ap, int);
        va_end (ap);

        return epoll_create(flags) ;
    }
    else if(sysno == __NR_epoll_ctl)
    { 
        va_list ap ;
        int epfd = 0 , op = 0 , fd = 0 ;
        struct epoll_event *ev = NULL ;
        va_start (ap, sysno);
        epfd = va_arg (ap, int);
        op = va_arg (ap, int);
        fd = va_arg (ap, int);
        ev = va_arg (ap, struct epoll_event *);
        va_end (ap);

        return epoll_ctl(epfd , op , fd , ev) ;
    }
    else if(sysno == __NR_epoll_wait)
    {
        va_list ap ;
        int epfd = 0 , maxevents = 0 , timeout = 0 ;
        struct epoll_event *evs = NULL ;
        va_start (ap, sysno);
        epfd = va_arg (ap, int);
        evs = va_arg (ap, struct epoll_event *);
        maxevents = va_arg (ap, int);
        timeout = va_arg (ap, int);        
        va_end (ap);

        return epoll_wait(epfd , evs , maxevents , timeout) ;
    }
    else if(sysno == __NR_inotify_init)
    {
        return inotify_init() ;
    }
    else if(sysno == __NR_inotify_add_watch)
    {
        va_list ap ;
        int fd = 0 ;
        const char * name = NULL ;
        uint32_t mask = 0;
        
        va_start (ap, sysno);
        fd = va_arg (ap, int);
        name = va_arg (ap, const char *);
        mask = va_arg (ap, uint32_t);
        va_end (ap);

        return inotify_add_watch(fd , name , mask) ;
    }
    else if(sysno == __NR_inotify_rm_watch)
    {
        va_list ap ;
        int fd = 0 ;
        uint32_t wd = 0;
        
        va_start (ap, sysno);
        fd = va_arg (ap, int);
        wd = va_arg (ap, uint32_t);
        va_end (ap);

        return inotify_rm_watch(fd , wd) ;
    }
    else
    {
        errno = ENOSYS ;
        return -1 ;
    }
}

