
#include <sys/sendfile.h>
#include <windows.h>
#include <winsock2.h>
#include <wintf/wobj.h>
#include <wintf/wcrt.h>

ssize_t sendfile(int out_fd , int in_fd , off_t * offset , size_t count)
{
    off64_t of64 = 0 ;
    ssize_t size = sendfile64(out_fd , in_fd , &of64 , count) ;
    if(offset != NULL)
        *offset = (off_t)of64 ;

    return size ;
}

ssize_t sendfile64(int out_fd , int in_fd , off64_t * offset , size_t count)
{
    wobj_t * out = wobj_get(out_fd) ;
    if(out == NULL || out->type != WOBJ_SOCK || out->handle == NULL)
        return 0 ;

    SOCKET s = (SOCKET)out->handle ;
    HANDLE in = (HANDLE)::_get_osfhandle(in_fd) ;

    char buffer[4096] ;
    size_t read_size = 0 ;

    while(read_size < count)
    {
        size_t size = count - read_size ;
        if(size > sizeof(buffer))
            size = sizeof(buffer) ;
        DWORD bytes = 0 ;
        if(::ReadFile(in , buffer , size , &bytes , NULL) == FALSE || bytes == 0)
            break ;

        int sent = ::_imp_send(s , buffer , bytes , 0) ;
        if(sent == SOCKET_ERROR || sent <= 0)
            break ;

        if(sent != bytes)
        {
            LONG dist = bytes - sent ;
            ::SetFilePointer(in , -dist , 0 , FILE_CURRENT) ;
        }

        read_size += sent ;
    }

    if(offset != NULL)
    {
        LONG high = 0 ;
        DWORD low = ::SetFilePointer(in , 0 , &high , FILE_CURRENT) ;

        off64_t of64 = high ;
        of64 = (of64 << 32) + low ;
        *offset = of64 ;
    }

    return read_size ;
}


