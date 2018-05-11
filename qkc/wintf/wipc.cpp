
#include "wintf/wipc.h"
#include "../internal/bitop.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

static const char * __wipc_mtx_name__   = "Global\\qkc.mtx" ;
static const char * __wipc_shm_name__   = "Global\\qkc.shm" ;
static const char * __ipc_super_magic__ = "qkc" ;

static HANDLE __wipc_mtx__ = NULL ;
static HANDLE __wipc_global_handle__ = NULL ;
static ipc_global_t * __wipc_global__ = NULL ;

static INIT_ONCE __wipc_inited__ = INIT_ONCE_STATIC_INIT ;
static SRWLOCK __wipc_rwlock__ =  SRWLOCK_INIT ;

static uint8_t  __version_major__ = 0 ;
static uint8_t  __version_minor__ = 1 ;
static uint16_t __version_build__ = (uint16_t)180511 ;


BOOL CALLBACK InitWIPCFunction(PINIT_ONCE InitOnce , PVOID Parameter , PVOID *lpContext)
{
    HANDLE handle = ::CreateMutex(NULL , FALSE , __wipc_mtx_name__) ;
    if(handle == NULL)
    {
        if(::GetLastError() == ERROR_ACCESS_DENIED)
            handle = ::OpenMutex(MUTEX_ALL_ACCESS , FALSE , __wipc_mtx_name__) ;   
    }

    __wipc_mtx__ = handle ;
    return (__wipc_mtx__ != NULL ? TRUE : FALSE);
}

BOOL ConfirmWIPCInited()
{  
    return InitOnceExecuteOnce(&__wipc_inited__,InitWIPCFunction,NULL, NULL);
}

const char * ipc_mtx_name()
{
    return __wipc_mtx_name__ ;
}

const char * ipc_shm_name()
{
    return __wipc_shm_name__ ;
}

uint32_t ipc_version() 
{
    uint32_t v = 0 ;//__version_build__ ;
    v =     (__version_major__ << 24);
    v |=    (__version_minor__ << 16);
    v |=    __version_build__;
    return v ;
}

bool ipc_super_validate_magic(ipc_super_t * super)
{
    return (::strcmp(super->magic , __ipc_super_magic__) == 0) ;
}

void ipc_super_assign_magic(ipc_super_t * super)
{
    ::memcpy(super->magic , __ipc_super_magic__ , 4) ;
}

bool ipc_global_init()
{
    if(ConfirmWIPCInited() == FALSE)
        return false ;

    if(::WaitForSingleObject(__wipc_mtx__ , INFINITE) != WAIT_OBJECT_0)
        return false ;

    bool result = false ;
    if(__wipc_global_handle__ == NULL)
        __wipc_global_handle__ = ::CreateFileMapping(INVALID_HANDLE_VALUE , NULL , PAGE_READWRITE , 0 , IPC_GLOBAL_SIZE , __wipc_shm_name__) ;
    if(__wipc_global_handle__ != NULL)
    {
        if(__wipc_global__ == NULL)
            __wipc_global__ = (ipc_global_t *)::MapViewOfFile(__wipc_global_handle__ , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , IPC_GLOBAL_SIZE) ;

        if(__wipc_global__ != NULL)
        {
            if(ipc_super_validate_magic((ipc_super_t *)__wipc_global__->super) == false)
            {
                ipc_super_t * super = (ipc_super_t *)__wipc_global__->super ;
                //没有被初始化
                ::memset(__wipc_global__ , 0 , IPC_GLOBAL_SIZE) ;
                ipc_super_assign_magic(super) ;
                super->ipc_size = IPC_GLOBAL_SIZE ;
                super->major = __version_major__ ;
                super->minor = __version_minor__ ;
                super->build = __version_build__ ;

                super->offset = IPC_SUPER_SIZE ;
                super->last_id = ((uint32_t)::time(NULL) << 16) ;

                result = true ;
            }
        }
    }

    ::ReleaseMutex(__wipc_mtx__) ;
    return true ;
}

bool ipc_global_final()
{
    HANDLE handle = __wipc_mtx__ ;
    __wipc_mtx__ = NULL ;

    if(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
    {
        ::UnmapViewOfFile(__wipc_global__) ;
        __wipc_global__ = NULL ;
        ::CloseHandle(__wipc_global_handle__) ;
        __wipc_global_handle__ = NULL ;

        ::ReleaseMutex(handle) ;
    }
    ::ReleaseMutex(handle) ;
    ::CloseHandle(handle) ;

    return true ;
}

int  ipc_alloc_id(key_t key , int type , int flag)
{
    if(::WaitForSingleObject(__wipc_mtx__ , INFINITE) != WAIT_OBJECT_0)
        return -1 ;

    ipc_super_t * super = (ipc_super_t *)__wipc_global__ ;

    uint32_t uidx = 0 , usize = (IPC_INFO_BITMAP_SIZE >> 2) ;
    uint32_t start_id = (super->last_id >> 14) , cur_id = (super->last_id & (IPC_ITEM_COUNT - 1)) ;
    uint32_t less_id = -1 , great_id = -1 ;

    uint32_t * bitmaps = (uint32_t *)super->bitmap ;
    ipc_item_t * items = __wipc_global__->items ;

    uint32_t offset = 0 ;
    int id = -1 ;
    bool found = false ;
    for(; (uidx < usize) && (found == false) ; ++uidx)
    {
        uint32_t uval = bitmaps[uidx] ;
        for(int bidx = 0 ; bidx < 32 ; ++bidx)
        {
            uint32_t mask = 1 << bidx ;
            if(bitop_in(uval , mask) == true)
            {
                //这个项是赋值过的，检查key是否一致
                offset = (uidx << 5) + bidx ;
                if(items[offset].key == key && items[offset].type == type)
                {
                    found = true ;
                    if(bitop_in(flag , IPC_CREAT) == true && bitop_in(flag , IPC_EXCL) == true)
                    {
                        errno = EEXIST ;
                        id = -1 ;
                    }
                    else
                    {
                        id = items[offset].id ;
                        items[offset].nattch++ ;
                    }
                    break ;
                }
            }
            else
            {
                uint32_t voff = (uidx << 5) + bidx ;
                if(less_id == -1)
                    less_id = voff ;
                if((great_id == -1) && ((voff & (IPC_ITEM_COUNT - 1)) > cur_id))
                    great_id = voff ;
            }
        }
    }

    if(found == false)
    {
        if(bitop_in(flag , IPC_CREAT) == true)
        {
            if(great_id == -1)
            {
                if(less_id != -1)
                {
                    id = less_id ;
                    start_id++ ;
                }
                else
                {
                    errno = ENOSPC ;
                }
            }
            else 
                id = great_id ;

            if(id != -1)
            {
                id = (start_id << 14) | (id & (IPC_ITEM_COUNT - 1)) ;
                ipc_item_t& item = items[id] ;
                item.key = key ;
                item.id = id ;
                item.type = type ;
                item.nattch = 1 ;

                ++super->count ;
                super->last_id = id ;
            }
        }
        else
        {
            errno = ENOENT ;
            id = -1 ;
        }
    }
    ::ReleaseMutex(__wipc_mtx__) ;
    return id ;
}

void  ipc_free_id(int id)
{
    if(::WaitForSingleObject(__wipc_mtx__ , INFINITE) != WAIT_OBJECT_0)
        return ;

    ipc_super_t * super = (ipc_super_t *)__wipc_global__ ;
    uint32_t real_id = (id & (IPC_ITEM_COUNT - 1)) ;

    ipc_item_t * items = __wipc_global__->items ;
    ipc_item_t *item = items + real_id ;
    
    item->nattch-- ;
    if(item->nattch == 0)
    {
        uint32_t * bitmaps = (uint32_t *)super->bitmap ;
        uint32_t uoff = (real_id >> 5) , ubits = (real_id & 31);

        ::bitop_clear(bitmaps[uoff] , (1 << ubits)) ;
        super->count-- ;
    }

    ::ReleaseMutex(__wipc_mtx__) ;
}

ipc_item_t * ipc_get_item_by_id(int id) 
{
    uint32_t real_id = (id & (IPC_ITEM_COUNT - 1)) ;
    return NULL ;
}

win_shm_t * ipc_shm_create(uint32_t id)
{
    ipc_item_t * item = ipc_get_item_by_id(id) ;
    if(item == NULL || item->id != id)
        return NULL ;

    win_shm_t * shm = (win_shm_t *)::malloc(sizeof(win_shm_t)) ;
    if(shm == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }
    ::memset(shm , 0 , sizeof(win_shm_t)) ;
    shm->key = item->key ;
    shm->id = item->id  ;
    shm->size = item->bytes ;

    if(ipc_shm_init(shm) == false)
    {
        ::free(shm) ;
        return NULL ;
    }
    return shm ;
}

bool ipc_shm_init(win_shm_t * shm)
{
    char name[256] = {'\0'} ;
    int nsize = ::sprintf(name , "%s.%u" , __wipc_shm_name__ , shm->id) + 1;

    HANDLE handle = ::CreateFileMapping(INVALID_HANDLE_VALUE , NULL , PAGE_READWRITE , 0 , shm->size , name) ;
    if(handle == NULL)
        return false ;
    void * addr = ::MapViewOfFile(handle , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , shm->size) ;
    if(addr == NULL)
    {
        ::CloseHandle(handle) ;
        return false ;
    }

    shm->fhandle = handle ;
    shm->map_addr = addr ;
    shm->page_protect = PAGE_READWRITE ;
    shm->map_access = FILE_MAP_READ | FILE_MAP_WRITE ;

    shm->name = (char *)::malloc(nsize) ;
    ::memcpy(shm->name , name , nsize) ;

    return true ;
}

bool ipc_shm_final(win_shm_t * shm)
{
    char * name = shm->name ;
    shm->name = NULL ;
    if(name != NULL)
        ::free(name) ;

    void * addr = shm->map_addr ;
    shm->map_addr = NULL ;
    if(addr != NULL)
        ::UnmapViewOfFile(addr) ;

    HANDLE handle = shm->fhandle ;
    shm->fhandle = NULL ;
    if(handle != NULL)
        ::CloseHandle(handle) ;

    return true ;
}

bool ipc_shm_destory(win_shm_t * shm)
{
    ipc_shm_final(shm) ;
    ipc_free_id(shm->id) ;
    ::free(shm) ;
    return true ;
}

