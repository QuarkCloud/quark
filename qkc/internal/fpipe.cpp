
#include "fpipe.h"
#include "iocp_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int iocp_pipe_callback(iocp_item_t * item , int evt , int result)
{
    if(item == NULL || item->addition == NULL)
        return 0 ;

    uint32_t events = item->data.events ;
    int old_occur = item->occur , new_occur = item->occur;

    if(evt == IOCP_EVENT_WRITE)
        iocp_process_event(events & EPOLLOUT , result , new_occur) ;
    else if(evt == IOCP_EVENT_READ)
        iocp_process_event(events & EPOLLIN , result , new_occur) ;
    else if(evt == IOCP_EVENT_CLOSE)
        new_occur = 0 ;
    item->occur = new_occur ;

    if((events & EPOLLET) != 0)
    {
        //边缘触发，只在状态翻转时，执行操作
        if(old_occur == 0 && new_occur != 0)
            iocp_mgr_item_ready(item->owner , item) ;
        else if(old_occur != 0 && new_occur == 0)
            iocp_mgr_item_unready(item->owner , item) ;
    }
    else
    {
        if(new_occur != 0)
            iocp_mgr_item_ready(item->owner , item) ;
        else
            iocp_mgr_item_unready(item->owner , item) ;
    }

    return 0 ;
}

void iocp_pipe_free(iocp_item_t * item)
{

}

pipe_t * pipe_new()
{
    pipe_t * data = (pipe_t *)::malloc(sizeof(pipe_t)) ;
    if(data == NULL)
        return NULL ;

    if(pipe_init(data) == true)
        return data ;

    ::free(data) ;
    return NULL ;
}

bool pipe_init(pipe_t * pipe)
{
    ::memset(pipe , 0 , sizeof(pipe_t)) ;
    pipe->locker = ::CreateMutex(NULL , FALSE , NULL) ;
    if(pipe->locker == INVALID_HANDLE_VALUE)
        return false ;
    return true ;
}

void pipe_free(pipe_t * pipe)
{
    pipe_final(pipe) ;
    if(pipe != NULL)
        ::free(pipe) ;
}

bool pipe_final(pipe_t * pipe)
{
    if(pipe == NULL)
        return false ;

    if(pipe->locker == INVALID_HANDLE_VALUE)
        return false ;

    iocp_item_t * item = (iocp_item_t *)pipe->addition ;
    if(item == NULL)
        return false ;

    iocp_pipe_callback(item , IOCP_EVENT_CLOSE , 0) ;

    if(::WaitForSingleObject(pipe->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    if(pipe->writer != NULL)
    {
        write_result_final(pipe->writer) ;
        pipe->writer = NULL ;
    }

    if(pipe->reader != NULL)
    {
        read_result_final(pipe->reader) ;
        pipe->reader = NULL ;
    }

    if(pipe->handle != NULL)
    {
        ::CloseHandle(pipe->handle) ;
        pipe->handle = NULL ;
    }

    ::ReleaseMutex(pipe->locker) ;
    ::CloseHandle(pipe->locker) ;
    pipe->locker = INVALID_HANDLE_VALUE ;

    return true ;
}

write_result_t * write_result_new()
{
    write_result_t * result = (write_result_t *)::malloc(sizeof(write_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM;
        return NULL ;
    }

    if(write_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void write_result_free(write_result_t * result)
{
    write_result_final(result) ;
    if(result != NULL)
        ::free(result) ;
}

bool write_result_init(write_result_t * result)
{
    if(result == NULL)
        return false ;

    ::memset(result , 0 , sizeof(write_result_t)) ;

    if(ring_buffer_init(&result->ring_buffer , PIPEBUFSIZE) == false)
    {
        errno = ENOMEM ;
        return false ;
    }  
    result->link.type = OVLP_OUTPUT ;
    return true ;
}

bool write_result_final(write_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }
    pipe_t * pipe = (pipe_t *)item->addition ;

    ::GetOverlappedResult(pipe->handle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    iocp_ovlp_unlock(&result->link) ;
    return true ;
}

bool pipe_write(write_result_t * result , int flags)
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    char * buf = NULL ;
    size_t size = 0 ;
    if(ring_buffer_refer_stream(&result->ring_buffer , buf , size) == false || buf == NULL || size == 0)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    DWORD write_bytes = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    pipe_t * pipe = (pipe_t *)item->addition ;

    int status = ::WriteFile(pipe->handle , buf , size , &write_bytes , &result->link.ovlp) ;
    if(status != 0)
    {
        int error = ::GetLastError() ;
        if(error != EWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_pipe_callback(result->link.owner , IOCP_EVENT_WRITE , error) ;
            return false ;
        }
    }

    return true ;
}

read_result_t * read_result_new()
{
    read_result_t * result = (read_result_t *)::malloc(sizeof(read_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(read_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void read_result_free(read_result_t * result)
{
    if(result == NULL)
        return ;

    read_result_final(result) ;
    ::free(result) ;
}

bool read_result_init(read_result_t * result)
{
    ::memset(result , 0 , sizeof(read_result_t)) ;
    result->link.type = OVLP_INPUT ;
    return true ;
}

bool read_result_final(read_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }
    pipe_t * pipe = (pipe_t *)item->addition ;

    ::GetOverlappedResult(pipe->handle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    iocp_ovlp_unlock(&result->link) ;
    return true ;
}

bool pipe_start_read(read_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    DWORD read_bytes = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }
    pipe_t * pipe = (pipe_t *)item->addition ;

    int status = ::ReadFile(pipe->handle , NULL , 0 ,  &read_bytes , &result->link.ovlp) ;
    if(status != 0)
    {
        int error = ::GetLastError() ;
        if(error != EWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_pipe_callback(result->link.owner , IOCP_EVENT_READ , error) ;
            return false ;
        }
        else
        {
            iocp_pipe_callback(result->link.owner , IOCP_EVENT_READ , EWOULDBLOCK) ;
        }
    }

    return true ;
}

