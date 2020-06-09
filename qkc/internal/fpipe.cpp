
#include "fpipe.h"
#include "iocp_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wintf/wobj.h"
#include "wobjs/CrtLinker.h"


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
	if (item != NULL && item->addition != NULL)
	{
		pipe_t * p = (pipe_t *)item->addition;
		item->addition = NULL;
	}
}

pipe_item_t * pipe_item_new()
{
	size_t item_size = sizeof(pipe_item_t);
	pipe_item_t * item = (pipe_item_t *)::malloc(item_size);
	if (item == NULL)
		return NULL;
	::memset(item, 0, item_size);
	return item;
}

void pipe_item_free(pipe_item_t * item)
{
	if (item == NULL)
		return;
	if (item->reader != NULL)
	{
		pipe_read_result_free(item->reader);
		item->reader = NULL;
	}

	if (item->writer != NULL)
	{
		pipe_write_result_free(item->writer);
		item->writer = NULL;
	}

	::free(item);
}

pipe_write_result_t * pipe_write_result_new()
{
	pipe_write_result_t * result = (pipe_write_result_t *)::malloc(sizeof(pipe_write_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM;
        return NULL ;
    }

    if(pipe_write_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void pipe_write_result_free(pipe_write_result_t * result)
{
	pipe_write_result_final(result) ;
    if(result != NULL)
        ::free(result) ;
}

bool pipe_write_result_init(pipe_write_result_t * result)
{
    if(result == NULL)
        return false ;

    ::memset(result , 0 , sizeof(pipe_write_result_t)) ;

    if(ring_buffer_init(&result->ring_buffer , PIPEBUFSIZE) == false)
    {
        errno = ENOMEM ;
        return false ;
    }  
    result->link.type = OVLP_OUTPUT ;
    return true ;
}

bool pipe_write_result_final(pipe_write_result_t * result)
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

bool pipe_write_iocp(pipe_write_result_t * result)
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

ssize_t pipe_write(pipe_item_t * pipe_item, const void * buf, size_t nbytes)
{
	if (pipe_item == NULL || buf == NULL || nbytes == 0 || pipe_item->writer == NULL)
		return -1;

	pipe_write_result_t * writer = pipe_item->writer;
	size_t write_bytes = ring_buffer_write_stream(&writer->ring_buffer, buf, nbytes);
	if (write_bytes == 0)
		return 0;

	pipe_write_iocp(writer);

	if (write_bytes == 0)
	{
		errno = EAGAIN;
		iocp_pipe_callback(pipe_item->iocp_item, IOCP_EVENT_WRITE, errno);
		return -1;
	}
	else
		return write_bytes;
}

pipe_read_result_t * pipe_read_result_new()
{
	pipe_read_result_t * result = (pipe_read_result_t *)::malloc(sizeof(pipe_read_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(pipe_read_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void pipe_read_result_free(pipe_read_result_t * result)
{
    if(result == NULL)
        return ;

	pipe_read_result_final(result) ;
    ::free(result) ;
}

bool pipe_read_result_init(pipe_read_result_t * result)
{
    ::memset(result , 0 , sizeof(pipe_read_result_t)) ;
    result->link.type = OVLP_INPUT ;
    return true ;
}

bool pipe_read_result_final(pipe_read_result_t * result)
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

bool pipe_start_read(pipe_read_result_t * result)
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
	pipe_item_t * pipe_item = (pipe_item_t *)item->addition ;
	pipe_t * pipe = pipe_item->pipe;

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

ssize_t pipe_read(pipe_item_t * pipe_item , void * buf, size_t nbytes)
{
	if (pipe_item == NULL || pipe_item->pipe == NULL)
		return -1;

	pipe_t * pipe = pipe_item->pipe;

	if (pipe->direct != PIPE_READER)
		return -1;


	DWORD read_bytes = 0;
	if (::ReadFile(pipe->handle, buf, nbytes, &read_bytes, NULL) == FALSE)
	{
		DWORD errcode = ::GetLastError();
		if (errcode == ERROR_IO_PENDING)
			return 0;
		else
			return -1;
	}

	pipe_start_read(pipe_item->reader);
	return read_bytes;
}

