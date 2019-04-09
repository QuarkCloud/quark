
#include "iocp_mgr.h"
#include "iocp_item.h"
#include "bitop.h"
#include <errno.h>
#include <wintf/wobj.h>
#include <wintf/wcrt.h>
#include <wintf/werr.h>
#include <windows.h>
#include <sys/socket.h>
#include "fsocket.h"
#include "fpipe.h"
#include "ffile.h"

iocp_mgr_t * iocp_mgr_new()
{
    iocp_mgr_t * mgr = (iocp_mgr_t *)::malloc(sizeof(iocp_mgr_t)) ;
    if(mgr == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    int epfd = INVALID_WOBJ_ID ;
    if((iocp_mgr_init(mgr) == true) && (epfd = wobj_set(WOBJ_IOCP , mgr->iocp , mgr)) != INVALID_WOBJ_ID)
    {
        mgr->epfd = epfd ;
        return mgr ;
    }
    else
    {
        ::free(mgr) ;
        return NULL ;
    }
}

int iocp_mgr_free(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return -1 ;

    int oid = mgr->epfd ;
    iocp_mgr_final(mgr) ;
    wobj_del(oid) ;
    ::free(mgr) ;
    return 0 ;
}

bool iocp_mgr_init(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;
    ::memset(mgr , 0 , sizeof(iocp_mgr_t)) ;
    mgr->iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(mgr->iocp == INVALID_HANDLE_VALUE)
    {
        errno = EIO ;
        return false ;
    }

    mgr->locker = ::CreateMutex(NULL , FALSE , NULL) ;

    rlist_init(&mgr->ready) ;

    return true ;
}

bool iocp_mgr_final(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;
   
    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    iocp_mgr_items_free(&mgr->ready) ;

    if(mgr->iocp != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(mgr->iocp) ;
        mgr->iocp = INVALID_HANDLE_VALUE ;
    }

    ::ReleaseMutex(mgr->locker) ;
    ::CloseHandle(mgr->locker) ;

    ::memset(mgr , 0 , sizeof(iocp_mgr_t)) ;
    return true ;
}

bool iocp_mgr_items_free(rlist_t * rlist) 
{
    if(rlist == NULL)
        return false ;
    rlist_t * next = NULL ;
    while((next = rlist->next) != rlist)
    {
        rlist_del(rlist , next) ;
        iocp_mgr_item_free((iocp_item_t *)next) ;
    }
    return true ;
}

bool iocp_mgr_item_free(iocp_item_t * item) 
{
    rlist_del(NULL , &item->link) ;
    iocp_item_free_t pfn_free = item->free ;
    if(pfn_free != NULL)
        pfn_free(item) ;
    else
        free(item) ;
    return true ;
}

bool iocp_mgr_item_ready(iocp_mgr_t * mgr , iocp_item_t * item) 
{
    if(mgr == NULL || item == NULL)
        return false ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;

    if(rlist_empty(&item->link) == true && item->occur != 0)
    {
        rlist_add_tail(&mgr->ready , &item->link) ;
        mgr->ready_count++ ;
    }    

    ::ReleaseMutex(mgr->locker) ;
    return true ;
}

bool iocp_mgr_item_unready(iocp_mgr_t * mgr , iocp_item_t * item) 
{
    if(mgr == NULL || item == NULL)
        return false ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;

    if(rlist_empty(&item->link) == false && item->occur == 0)
    {
        rlist_del(NULL , &item->link) ;
        mgr->ready_count-- ;
    }

    ::ReleaseMutex(mgr->locker) ;
    return true ;
}

int iocp_mgr_retrieve(iocp_mgr_t * mgr , struct epoll_event * evs ,  int maxevents)
{
    if(mgr == NULL || evs == NULL || maxevents <= 0)
        return -1 ;

    int counter = 0 ;
    ::WaitForSingleObject(mgr->locker , INFINITE) ;

    rlist_t * head = &mgr->ready , *next = NULL ;
    while((counter < maxevents) && (next = head->next) != head)
    {
        rlist_del(NULL , next) ;
        iocp_item_t * item = (iocp_item_t *)next ;
        mgr->ready_count-- ;        
        evs[counter].data.u64 = item->data.data.u64 ;
        evs[counter].events = item->occur ;
        item->occur = 0 ;
        ++counter ;
    }

    ::ReleaseMutex(mgr->locker) ;
    return counter ;
}

bool iocp_mgr_add(iocp_mgr_t * mgr , int fd , struct epoll_event * ev)
{
    if(mgr == NULL || fd <= 0 || ev == NULL)
        return false ;

    wobj_t * wobj = ::wobj_get(fd) ;
    if(wobj == NULL)
        return false ;
    wobj_type wtype = wobj->type ;
    if(wtype != WOBJ_SOCK && wtype != WOBJ_FILE && wtype != WOBJ_PIPE)
        return false ;

    iocp_item_t * item = (iocp_item_t *)::malloc(sizeof(iocp_item_t)) ;
    if(item == NULL)
        return false ;
    ::memset(item , 0 , sizeof(iocp_item_t)) ;

    item->fd = fd ;
    item->owner = mgr ;
    rlist_init(&item->link) ;
    ::memcpy(&item->data , ev , sizeof(struct epoll_event)) ;

    if(wtype == WOBJ_SOCK)
    {
        socket_t * s= (socket_t *)wobj->addition ;
        s->addition = item ;

        item->type = IOCP_ITEM_SOCKET ;
        item->callback = iocp_socket_callback ;
        item->free = iocp_socket_free ;
        item->addition = s ;

        //绑定到iocp中
        if(::CreateIoCompletionPort((HANDLE)s->socket , mgr->iocp , 0 , 0) != NULL)
        {
            if((ev->events & EPOLLIN) == EPOLLIN)
            {
                if(s->stage == SOCKET_STAGE_LISTEN)
                    socket_start_accept(s->acceptor) ;
                else if(s->stage == SOCKET_STAGE_CONNECT && s->type == SOCK_STREAM)
                    ::socket_start_recv(s->receiver) ;
                else if(s->type == SOCK_DGRAM)
                {
                    struct sockaddr addr ;
                    int addr_len = sizeof(struct sockaddr) ;
                    ::socket_start_recvfrom(s->receiver , 0 , &addr , &addr_len) ;
                }
            }

            return true ;
        }
    }
	else if (wtype == WOBJ_PIPE)
	{
		pipe_t * p = (pipe_t *)wobj->addition;
		pipe_item_t * pipe_item = pipe_item_new();
		p->addition = pipe_item;
		pipe_item->pipe = p;
		pipe_item->iocp_item = item;
		item->type = IOCP_ITEM_PIPE;
		item->callback = iocp_pipe_callback;
		item->free = iocp_pipe_free;
		item->addition = pipe_item ;

		//绑定到IOCP中
		if (::CreateIoCompletionPort(p->handle , mgr->iocp , 0 , 0) != NULL)
		{
			if (((ev->events & EPOLLIN) == EPOLLIN) && (p->direct == PIPE_READER))
			{
				if (pipe_item->reader == NULL)
				{
					pipe_item->reader = pipe_read_result_new();
					pipe_item->reader->link.owner = item;
				}
				::pipe_start_read(pipe_item->reader);
			}
			else if (((ev->events & EPOLLOUT) == EPOLLOUT) && (p->direct == PIPE_WRITER))
			{
				if (pipe_item->writer == NULL)
				{
					pipe_item->writer = pipe_write_result_new();
					pipe_item->writer->link.owner = item;
				}
			}

			return true;
		}
	}
	else if (wtype == WOBJ_FILE)
	{

		file_t * f = (file_t *)wobj->addition;
		f->addition = item;

		item->type = IOCP_ITEM_SOCKET;
		item->callback = iocp_file_callback;
		item->free = iocp_file_free;
		item->addition = f;

		//绑定到iocp中
		if (::CreateIoCompletionPort(f->handle, mgr->iocp, 0, 0) != NULL)
		{
			if ((ev->events & EPOLLIN) == EPOLLIN)
			{
				::file_start_read(f->reader);
			}

			return true;
		}
		else
		{
			DWORD errcode = ::GetLastError();
			::printf("errcode = %u \n" , errcode);
		}
	}

    ::free(item) ;
    return false ;
}

bool iocp_mgr_mod(iocp_mgr_t * mgr , int fd , struct epoll_event * ev)
{
    if(mgr == NULL)
        return false ;

    wobj_t * wobj = ::wobj_get(fd) ;
    if(wobj == NULL || wobj->addition == NULL)
        return false ;

    iocp_item_t * item = NULL ;
    if(wobj->type == WOBJ_SOCK)
    {   
        socket_t * s = (socket_t *)wobj->addition ;
        item = (iocp_item_t *)s->addition ;
    }
	else if (wobj->type == WOBJ_PIPE)
	{
		pipe_item_t * p = (pipe_item_t *)wobj->addition;
		item = p->iocp_item;
	}

    if(item == NULL || item->fd != fd)
        return false ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    rlist_del(NULL , &item->link) ;
    ::memcpy(&item->data , ev , sizeof(struct epoll_event)) ;
    ::ReleaseMutex(mgr->locker) ;
    
    return true ;
}

bool iocp_mgr_del(iocp_mgr_t * mgr , int fd , struct epoll_event * ev)
{
    if(mgr == NULL)
        return false ;

    wobj_t * wobj = ::wobj_get(fd) ;
    if(wobj == NULL || wobj->addition == NULL)
        return false ;

    iocp_item_t * item = NULL ;
    if(wobj->type == WOBJ_SOCK)
    {   
        socket_t * s = (socket_t *)wobj->addition ;
        item = (iocp_item_t *)s->addition ;
        s->addition = NULL ;
    }
	else if (wobj->type == WOBJ_PIPE)
	{
		pipe_t * p = (pipe_t *)wobj->addition;
		pipe_item_t * pi = (pipe_item_t *)p->addition;
		item = pi->iocp_item;
		p->addition = NULL;
		pipe_item_free(pi);
	}
	else if (wobj->type == WOBJ_FILE)
	{
		file_t * f = (file_t *)wobj->addition;
		if (f != NULL)
		{
			item = (iocp_item_t *)f->addition;
			f->addition = NULL;
		}
	}

    if(item == NULL)
        return true ;

    item->addition = NULL ;
    item->callback = NULL ;
    item->free = NULL ;
    item->fd = -1 ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    rlist_del(NULL , &item->link) ;
    iocp_mgr_item_free(item) ;
    ::ReleaseMutex(mgr->locker) ;

    return true ;
}

int iocp_mgr_wait(iocp_mgr_t * mgr , int timeout) 
{
    DWORD start_time = ::GetTickCount() ;
    DWORD bytes_transferred = 0 ;
    ULONG_PTR completion_key = 0 ;
    LPOVERLAPPED overlapped = NULL ;
    
    ::InterlockedIncrement(&mgr->thread_counter) ;
    BOOL result = ::GetQueuedCompletionStatus(mgr->iocp , &bytes_transferred , &completion_key , &overlapped , timeout) ;
    LONG now_threads =  ::InterlockedDecrement(&mgr->thread_counter) ;

    if(result == FALSE && overlapped == NULL)
    {
        //没有可等待的事件
        return -1 ;
    }

    iocp_ovlp_t * ovlp = (iocp_ovlp_t *)overlapped ;
    iocp_item_t * item = (ovlp == NULL) ? NULL : ovlp->owner ;

    uint32_t old_occur = 0 , new_occur = 0 ;
    uint32_t events = 0 ;
    if(item != NULL)
    {
        old_occur = item->occur ;
        events = item->data.events ;
    }

    if(result == TRUE)
    {
        if(overlapped == NULL && completion_key == 1)
        {
            if(now_threads > 0)
                ::PostQueuedCompletionStatus(mgr->iocp , 0 , completion_key , NULL) ;
            return 0 ;
        }

        ovlp_type_t type = ovlp->type ;
        if(type == OVLP_INPUT)
        {
            if(bitop_in(events , EPOLLIN) == true)
            {
                //标记可读
                new_occur |= EPOLLIN ;
            }        
        }
        else if(type == OVLP_OUTPUT)
        {
            if(bitop_in(events , EPOLLOUT) == true)
            {
                //标记可写
                new_occur |= EPOLLOUT ;
            }        
        }
    }
    else
    {
        if((item != NULL) && (bitop_in(events , EPOLLERR) == true))
        {
            new_occur |= EPOLLERR ;
        }        
    }

    bitop_set(item->occur , new_occur) ;
    new_occur = item->occur ;
    iocp_ovlp_unlock(ovlp) ;

    bool is_et = bitop_in(events , EPOLLET) ;

    if(old_occur == new_occur)
    {
        if(new_occur != 0 && is_et == false)
            iocp_mgr_item_ready(mgr , item) ;
    }
    else
    {
        if(new_occur == 0)
            iocp_mgr_item_unready(mgr , item) ;
        else if(is_et == false)
            iocp_mgr_item_ready(mgr , item) ;
    }

    return 0 ;
}

void iocp_process_event(int events , int result , int& occur)
{
    if(events == 0)
        return ;

    int old_occur = 0 , now_occur = 0;
    if(result == 0)
    {
        old_occur = (occur & events) ;
        now_occur |= events ;
    }
    else if(result != WSAEWOULDBLOCK) 
    {
        old_occur = (occur & events) ;
        now_occur |= events ;
    } 
    else if(result == WSAEWOULDBLOCK)
    {
        old_occur = (occur & events) ;
        now_occur = (old_occur ^ events) ;
    }

    occur = (occur & (~events)) | now_occur ;
}

