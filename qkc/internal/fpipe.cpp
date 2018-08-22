
#include "fpipe.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>


void pipe_callback(pipe_t *pipe , int evt , int result)
{
    if(pipe == NULL || pipe->callback == NULL)
        return ;

    pipe_callback_t callback = pipe->callback ;
    callback(pipe , evt , result) ;
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

    pipe_callback(pipe , kPipeBeforeClose , 0) ;

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

    if(pipe->rhandle != NULL)
    {
        ::CloseHandle(pipe->rhandle) ;
        pipe->rhandle = NULL ;
    }

    if(pipe->whandle != NULL)
    {
        ::CloseHandle(pipe->whandle) ;
        pipe->whandle = NULL ;
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
    result->link.type = OVLP_PIPE_OUTPUT ;
    return true ;
}

bool write_result_final(write_result_t * result)
{
    if(pipe_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 ;

    ::GetOverlappedResult(result->link.owner->rhandle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    ::GetOverlappedResult(result->link.owner->whandle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    pipe_ovlp_unlock(&result->link) ;
    return true ;
}

bool pipe_write(write_result_t * result , int flags)
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(pipe_ovlp_lock(&result->link) == false)
        return false ;

    char * buf = NULL ;
    size_t size = 0 ;
    if(ring_buffer_refer_stream(&result->ring_buffer , buf , size) == false || buf == NULL || size == 0)
    {
        pipe_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    DWORD write_bytes = 0 ;
    int status = ::WriteFile(result->link.owner->whandle , buf , size , &write_bytes , &result->link.ovlp) ;
    if(status != 0)
    {
        int error = ::GetLastError() ;
        if(error != EWOULDBLOCK)
        {
            result->link.status = error ;
            pipe_ovlp_unlock(&result->link) ;
            pipe_callback(result->link.owner , kPipeWrite , error) ;
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
    result->link.type = OVLP_PIPE_INPUT ;
    return true ;
}

bool read_result_final(read_result_t * result)
{
    if(pipe_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    ::GetOverlappedResult(result->link.owner->rhandle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    ::GetOverlappedResult(result->link.owner->whandle , &result->link.ovlp , & bytes_transfer , TRUE) ;
    pipe_ovlp_unlock(&result->link) ;
    return true ;
}

bool pipe_start_read(read_result_t * result)
{
    if(pipe_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    DWORD read_bytes = 0 ;
    int status = ::ReadFile(result->link.owner->rhandle , NULL , 0 ,  &read_bytes , &result->link.ovlp) ;
    if(status != 0)
    {
        int error = ::GetLastError() ;
        if(error != EWOULDBLOCK)
        {
            result->link.status = error ;
            pipe_ovlp_unlock(&result->link) ;
            pipe_callback(result->link.owner , kPipeRead , error) ;
            return false ;
        }
        else
        {
            pipe_callback(result->link.owner , kPipeRead , EWOULDBLOCK) ;
        }
    }

    return true ;
}

bool pipe_ovlp_lock(pipe_ovlp_t * ovlp)
{
    return (::InterlockedCompareExchange(&ovlp->counter , 1 , 0) == 0) ;
}

bool pipe_ovlp_unlock(pipe_ovlp_t * ovlp)
{
    return (::InterlockedCompareExchange(&ovlp->counter , 0 , 1) == 1) ;
}

int pipe_ovlp_counter(pipe_ovlp_t * ovlp)
{
    return (int)::InterlockedCompareExchange(&ovlp->counter , 0 , 0) ;
}


