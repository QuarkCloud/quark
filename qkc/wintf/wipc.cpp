
#include <wintf/wipc.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>

static const char * __wipc_name__ = "Global\\qkc.ipc" ;
static const char * __wipc_glmtx_name__ = "Global\\qkc.ipc.glmtx" ;
static const char * __wipc_glshm_name__ = "Global\\qkc.ipc.glshm" ;

const char * ipc_name() 
{
    return __wipc_name__ ;
}

const char * ipc_glmtx_name()
{
    return __wipc_glmtx_name__ ;
}

const char * ipc_glshm_name()
{
    return __wipc_glshm_name__ ;
}


int ipc_item_get_type(const ipc_item_head_t * head)
{
    char type = head->magic[0] ;
    if(type <= 0  || type >= IPC_TYPE_MAX)
        return IPC_TYPE_UNK ;

    const char * desc = head->magic + 1 ;
    const char * names = ipc_type_names[type] ;
    if(desc[0] == names[0] && desc[1] == names[1] && desc[2] == names[2])
        return type ;
    else
        return IPC_TYPE_UNK ; 
}

bool ipc_item_set_type(ipc_item_head_t * head , int type)
{
    if(head == NULL || (type <= 0 || type >= IPC_TYPE_MAX))
        return false ;

    char * magic = head->magic ;
    const char * names = ipc_type_names[type] ;
    magic[0] = (char)type ;
    magic[1] = names[0] ;
    magic[2] = names[1] ;
    magic[3] = names[2] ;

    return true ;
}

static ipc_section_t * __global_ipc_section__ = NULL ;
static HANDLE __global_ipc_section_filemap__ = NULL ;
static HANDLE __global_ipc_section_mutex__ = NULL ;

ipc_section_t * ipc_section_init() 
{
    if(__global_ipc_section__ != NULL)
        return __global_ipc_section__ ;

    //double check
    global_wrlock() ;
    if(__global_ipc_section__ != NULL)
    {
        global_wrunlock() ;
        return __global_ipc_section__ ;
    }

    if(__global_ipc_section_mutex__ == NULL)
        __global_ipc_section_mutex__ = ::CreateMutex(NULL , FALSE , __wipc_glmtx_name__) ;        

    if(::WaitForSingleObject(__global_ipc_section_mutex__ , INFINITE) == WAIT_OBJECT_0)
    {
        __global_ipc_section_filemap__ = ::CreateFileMapping(INVALID_HANDLE_VALUE , NULL , PAGE_READWRITE , 0 , IPC_SECTION_SIZE , __wipc_glshm_name__) ;
        void * addr = ::MapViewOfFile(__global_ipc_section_filemap__ , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , IPC_SECTION_SIZE) ;
        if(addr != NULL)
        {
            ipc_section_head_t * head = (ipc_section_head_t *)addr ;
            char * magic = (char *)head->magic ;
            if((magic[0] != IPC_HEAD_MAGIC[0]) || (magic[1] != IPC_HEAD_MAGIC[1]) || (magic[2] != IPC_HEAD_MAGIC[2]))
            {
                //没有被初始化
                ::memset(head , 0 , IPC_SECTION_SIZE) ;
                ::memcpy(magic , IPC_HEAD_MAGIC , 4) ;

                head->start = 1 ;
                head->last_id = 1 ;
            }
            __global_ipc_section__ = (ipc_section_t * ) addr ;
        }

        ::ReleaseMutex(__global_ipc_section_mutex__) ;
    }

    global_wrunlock() ;
    return __global_ipc_section__ ;
}

ipc_section_t * ipc_section_get()
{
    return __global_ipc_section__ ;
}

void ipc_section_final() 
{
    global_wrlock() ;

    HANDLE handle = __global_ipc_section_mutex__ ;
    __global_ipc_section_mutex__ = NULL ;

    if(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
    {
        ::UnmapViewOfFile(__global_ipc_section__) ;
        __global_ipc_section__ = NULL ;
        ::CloseHandle(__global_ipc_section_filemap__) ;
        __global_ipc_section_filemap__ = NULL ;

        ::ReleaseMutex(handle) ;
    }

    ::CloseHandle(handle) ;
    global_wrunlock() ;
}

