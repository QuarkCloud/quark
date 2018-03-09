
#include <sys/epoll.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <winsock2.h>
#include <errno.h>
#include <inner/rlist.h>

typedef struct _st_epoll{
    

} epoll_t;

int epoll_create (int size)
{
    HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(handle == INVALID_HANDLE_VALUE)
    {
        errno = EIO ;
        return -1 ;
    }

    return wobj_set(WOBJ_IOCP , handle , NULL) ;
}

int epoll_ctl (int epfd, int op, int fd, struct epoll_event * ev)
{
    wobj_t * eobj = ::wobj_get(epfd) ;
    if(eobj == NULL || eobj->type != WOBJ_IOCP)
        return -1 ;
    
    

}

int epoll_wait (int epfd, struct epoll_event * evs ,  int maxevents, int timeout)
{
    return 0 ;
}
