
#include <wintf/wipc.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>
#include "../inner/hash.h"
#include <sys/ipc.h>

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

ipc_item_t * ipc_item_alloc(int type , const char * name)
{
    if(type != IPC_TYPE_SHM && type != IPC_TYPE_SEM)
        return NULL ;

    uint32_t hash_value = 0 ;
    hash_value = hash_time33(name , -1 , hash_value) ;

    ipc_section_t * ipc = ipc_section_init()  ;

    ipc_item_t * item = NULL ;
    if(::WaitForSingleObject(__global_ipc_section_mutex__ , INFINITE) != WAIT_OBJECT_0)
        return NULL ;

    ipc_section_head_t * section = (ipc_section_head_t *)ipc->items ;

    for(int idx = 1 ; idx < IPC_ITEM_COUNT ; ++idx)
    {        
        ipc_item_head_t * head = (ipc_item_head_t *)(ipc->items + idx) ;
        int item_type = ipc_item_get_type(head) ;
        if(item_type == type && head->hash == hash_value)
        {
            //已经存在
            item = ipc->items + idx ;
            break ;
        }

        if(item_type != IPC_TYPE_UNK)
            continue ;

        //这是为空的，可以分配
        ::ipc_item_set_type(head , type) ;
        if(type == IPC_TYPE_SHM)
        {
            ipc_shm_item_t * shm = (ipc_shm_item_t *)head ;
            shm->key = ::ftok(name , 0) ;
            shm->shmid = ++section->last_id ;
            section->count++ ;

            size_t nsize = ::strlen(name) + 1 ;
            ::memcpy(shm->name , name , nsize) ;        //溢出，暂时不管
        }
        else if(type == IPC_TYPE_SEM)
        {
            ipc_sem_item_t * sem = (ipc_sem_item_t *)head ;
            sem->key = ::ftok(name , 0) ;
            sem->semid = ++section->last_id ;
            section->count++ ;

            size_t nsize = ::strlen(name) + 1 ;
            ::memcpy(sem->name , name , nsize) ;            
        }

        item = ipc->items + idx ;
        break ;
    }

    ::ReleaseMutex(__global_ipc_section_mutex__) ;

    return item ;
}

bool ipc_item_free(ipc_item_t * item)
{
    if(::WaitForSingleObject(__global_ipc_section_mutex__ , INFINITE) != WAIT_OBJECT_0)
        return false ;
    ::memset(item , 0 , IPC_ITEM_SIZE) ;

    ipc_section_head_t * head = (ipc_section_head_t *)__global_ipc_section__->items ;
    --head->count ;

    ::ReleaseMutex(__global_ipc_section_mutex__) ;
    return true ;
}

bool ipc_item_attach(ipc_item_t * item)
{
    if(::WaitForSingleObject(__global_ipc_section_mutex__ , INFINITE) != WAIT_OBJECT_0)
        return false ;

    ipc_item_head_t * head = (ipc_item_head_t *)item ;
    int type = ipc_item_get_type(head) ;
    if(type == IPC_TYPE_SHM)
    {
        ipc_shm_item_t * shm = (ipc_shm_item_t *)item ;
        ++shm->nattch ;
    }
    else if(type == IPC_TYPE_SEM)
    {
        ipc_sem_item_t * sem = (ipc_sem_item_t *)item ;
        ++sem->nattch ;
    }

    ::ReleaseMutex(__global_ipc_section_mutex__) ;
    return true ;
}

bool ipc_item_detach(ipc_item_t * item)
{
    if(::WaitForSingleObject(__global_ipc_section_mutex__ , INFINITE) != WAIT_OBJECT_0)
        return false ;

    ipc_item_head_t * head = (ipc_item_head_t *)item ;
    int type = ipc_item_get_type(head) ;
    if(type == IPC_TYPE_SHM)
    {
        ipc_shm_item_t * shm = (ipc_shm_item_t *)item ;
        --shm->nattch ;
    }
    else if(type == IPC_TYPE_SEM)
    {
        ipc_sem_item_t * sem = (ipc_sem_item_t *)item ;
        --sem->nattch ;
    }

    ::ReleaseMutex(__global_ipc_section_mutex__) ;
    return true ;
}


