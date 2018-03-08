
#include <sys/epoll.h>
#include <wintf/wobj.h>
#include <windows.h>

int epoll_create (int size)
{
    return 0 ;
}

int epoll_ctl (int epfd, int op, int fd, struct epoll_event * ev)
{
    return 0 ;
}

int epoll_wait (int epfd, struct epoll_event * evs ,  int maxevents, int timeout)
{
    return 0 ;
}
