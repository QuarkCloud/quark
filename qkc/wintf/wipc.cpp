
#include "wintf/wipc.h"
#include "../internal/bitop.h"
#include "../internal/rbtree.h"
#include "../internal/addr_mgr.h"
#include <sys/ipc.h>
#include <limits.h>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

static const char * __wipc_mtx_name__   = "Global\\qkc.mtx" ;
static const char * __wipc_shm_name__   = "Global\\qkc.shm" ;
static const char * __wipc_sem_name__   = "Global\\qkc.sem" ;
static const char * __ipc_super_magic__ = "qkc" ;

static HANDLE __wipc_mtx__ = NULL ;
static HANDLE __wipc_global_handle__ = NULL ;
static ipc_global_t * __wipc_global__ = NULL ;

static INIT_ONCE __wipc_inited__ = INIT_ONCE_STATIC_INIT ;
static SRWLOCK __wipc_rwlock__ =  SRWLOCK_INIT ;

static addr_mgr_t __wipc_addr_mgr__ = ADDR_MGR_INIT ;

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

const char * ipc_sem_name() 
{
    return __wipc_sem_name__ ;
}

typedef struct __st_ipc_info{

} ipc_info_t ;

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

void ipc_super_bitmap_set(ipc_super_t * super , uint32_t id , bool val) 
{
    uint32_t uidx = (id >> 5) , ubits = (id & 31);
    uint32_t * u32s = (uint32_t *)super->bitmap ;

    if(val == true)
        bitop_set(u32s[uidx] , (1<<ubits)) ;
    else
        bitop_clear(u32s[uidx] , (1<<ubits)) ;
}

void ipc_super_bitmap_get(ipc_super_t * super , uint32_t id , bool& val)
{
    uint32_t uidx = (id >> 5) , ubits = (id & 31);
    uint32_t * u32s = (uint32_t *)super->bitmap ;

    val = (::bitop_get(u32s[uidx] , (1<<ubits)) != 0) ;
}

bool ipc_global_init()
{
    if(ConfirmWIPCInited() == FALSE)
        return false ;

    if(__wipc_global_handle__ != NULL)
        return true ;

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
    if(ipc_global_init() == false)
        return -1 ;

    if(::WaitForSingleObject(__wipc_mtx__ , INFINITE) != WAIT_OBJECT_0)
        return -1 ;

    ipc_super_t * super = (ipc_super_t *)__wipc_global__ ;

    uint32_t start_id = (super->last_id >> 14) , cur_id = (super->last_id & (IPC_ITEM_COUNT - 1)) ;
    uint32_t less_id = -1 , great_id = -1 ;

    uint32_t * bitmaps = (uint32_t *)super->bitmap ;
    ipc_item_t * items = __wipc_global__->items ;

    uint32_t offset = 0 ;
    int id = -1 ;
    bool found = false ;

    if(key == IPC_PRIVATE)
    {
        uint32_t sid = cur_id + 1, count = 0;
        while(count < IPC_ITEM_COUNT)
        {
            bool val = false ;
            ipc_super_bitmap_get(super , sid , val) ;
            if(val == false)
            {
                great_id = (sid & (IPC_ITEM_COUNT - 1)) ;
                start_id = (sid >> 14) ;
                break ;
            }

            ++count ;
            ++sid ;
        }
    }
    else
    {
        uint32_t uidx = 0 , usize = (IPC_INFO_BITMAP_SIZE >> 2) ;
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
                ipc_super_bitmap_set(super , id , true) ;
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
    if(ipc_global_init() == false)
        return ;

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
    uint32_t uof = (real_id >> 5) , ubits = (real_id & 31);
    uint32_t * u32s = (uint32_t *)((ipc_super_t *)__wipc_global__->super)->bitmap ;

    if(::bitop_in(u32s[uof] , (1<<ubits)) == false)
        return NULL ;

    return (__wipc_global__->items + real_id) ;
}

win_shm_t * ipc_shm_create(uint32_t shmid , size_t size)
{
    ipc_item_t * item = ipc_get_item_by_id(shmid) ;
    if(item == NULL || item->id != shmid)
        return NULL ;

    win_shm_t * shm = (win_shm_t *)::malloc(sizeof(win_shm_t)) ;
    if(shm == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }
    ::memset(shm , 0 , sizeof(win_shm_t)) ;
    shm->key = item->key ;
    shm->shmid = item->id  ;
    if(((ipc_shm_t *)item)->bytes == 0)
        shm->size = size ;
    else
        shm->size = ((ipc_shm_t *)item)->bytes ;
    shm->ipc = (ipc_shm_t *)item ;

    return shm ;
}

bool ipc_shm_init(win_shm_t * shm)
{
    char name[256] = {'\0'} ;
    int nsize = ::sprintf(name , "%s.%u" , __wipc_shm_name__ , shm->shmid) + 1;

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

bool ipc_shm_destroy(win_shm_t * shm)
{
    ipc_shm_final(shm) ;
    ipc_free_id(shm->shmid) ;
    ::free(shm) ;
    return true ;
}

void ipc_shm_item_free(void * addition)
{
    win_shm_t * shm = (win_shm_t *)addition ;
    if(shm == NULL)
        return ;

    ipc_shm_destroy(shm) ;
}

bool ipc_shm_addr_add(void * addr , win_shm_t * shm)
{
    return addr_mgr_add(&__wipc_addr_mgr__ , addr , shm , ipc_shm_item_free) ;
}

win_shm_t * ipc_shm_addr_find(const void * addr) 
{
    addr_item_t * item = addr_mgr_find(&__wipc_addr_mgr__ , addr) ;
    if(item == NULL)
        return NULL ;

    return (win_shm_t *)item->addition ;
}

bool ipc_shm_addr_del(const void * addr)
{
    addr_item_t * item = addr_mgr_find(&__wipc_addr_mgr__ , addr) ;
    if(item == NULL)
        return false ;

    return addr_mgr_del(&__wipc_addr_mgr__ , item) ;
}

win_sem_t * ipc_sem_create(uint32_t semid)
{
    ipc_item_t * item = ipc_get_item_by_id(semid) ;
    if(item == NULL)
        return NULL ;

    win_sem_t * sem = (win_sem_t *)::malloc(sizeof(win_sem_t)) ;
    if(sem == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }
    ::memset(sem , 0 , sizeof(win_sem_t)) ;

    sem->key = item->key ;
    sem->semid = item->id ;
    sem->ipc = (ipc_sem_t *)item ;    
    return sem ;    
}

bool ipc_sem_init(win_sem_t * sem)
{
    char name[256] = {'\0'} ;
    int nsize = ::sprintf(name , "%s.%u" , __wipc_sem_name__ , sem->semid) + 1;

    HANDLE handle = ::CreateSemaphore(NULL , 0 , LONG_MAX , name) ;
    if(handle == NULL)
        return false ;

    sem->handle = handle ;
    char * nstr = (char *)::malloc(nsize) ;
    if(nstr != NULL)
    {
        ::memcpy(sem->name , name , nsize) ;
        sem->name = nstr ;
    }
    return true ;
}

bool ipc_sem_final(win_sem_t * sem)
{
    if(sem == NULL)
        return false ;

    char * name = sem->name ;
    sem->name = NULL ;
    HANDLE handle = sem->handle ;
    sem->handle = NULL ;

    if(name != NULL)
        ::free(name) ;
    if(handle != NULL)
        ::CloseHandle(handle) ;
    return true ;
}

bool ipc_sem_destroy(win_sem_t * sem)
{
    if(sem == NULL)
        return false ;

    ipc_sem_final(sem) ;
    ::free(sem) ;
    return true ;
}

