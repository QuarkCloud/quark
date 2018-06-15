
#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>
#include "internal/rlist.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

DWORD __mmap_prot_to_win(int prot)
{
    DWORD wp = 0 ;
    prot &= 0x7 ;
    if(prot == PROT_READ)
        wp = PAGE_READONLY ;
    else if(prot == PROT_WRITE)
        wp = PAGE_WRITECOPY ;
    else if(prot == PROT_EXEC)
        wp = PAGE_EXECUTE_READ ;
    else if(prot == (PROT_READ | PROT_WRITE))
        wp = PAGE_READWRITE ;
    else if(prot == (PROT_READ | PROT_EXEC))
        wp = PAGE_EXECUTE_READ ;
    else if(prot == (PROT_WRITE | PROT_EXEC))
        wp = PAGE_EXECUTE_WRITECOPY ;
    else if(prot == (PROT_READ | PROT_WRITE | PROT_EXEC))
        wp = PAGE_EXECUTE_READWRITE ;

    return wp ;
}

DWORD __mmap_flag_to_win(int flags)
{
//#define MAP_SHARED	0x01		/* Share changes.  */
//#define MAP_PRIVATE	0x02		/* Changes are private.  */
//#define MAP_TYPE	0x0f		/* Mask for type of mapping.  */

//#define MAP_FIXED	0x10		/* Interpret addr exactly.  */
//# define MAP_FILE	0
//# define MAP_ANONYMOUS	0x20		/* Don't use a file.  */
//# define MAP_ANON	MAP_ANONYMOUS
//# define MAP_32BIT	0x40		/* Only give out 32-bit addresses.  */

//# define MAP_GROWSDOWN	0x00100		/* Stack-like segment.  */
//# define MAP_DENYWRITE	0x00800		/* ETXTBSY */
//# define MAP_EXECUTABLE	0x01000		/* Mark it as an executable.  */
//# define MAP_LOCKED	0x02000		/* Lock the mapping.  */
//# define MAP_NORESERVE	0x04000		/* Don't check for reservations.  */
//# define MAP_POPULATE	0x08000		/* Populate (prefault) pagetables.  */
//# define MAP_NONBLOCK	0x10000		/* Do not block on IO.  */
//# define MAP_STACK      0x20000         /* Allocation is for a stack.  */

    //这些标识还蛮多的，后面再处理。
    return 0 ;
}

typedef struct __st_mmap_info{
    rlist_t link ;
    int file_desc ;
    int prot ;
    int flags ;
    size_t len ;
    off_t offset ;
    void * expect_addr ;

    HANDLE file_handle ;
    HANDLE map_handle ;
    void * map_addr ;

    //用于munmap
    void * use_addr ;
    size_t use_size ;

    char * name ;
} mmap_info_t ;

//偷个懒，用链表管理。因为mmap本身就不会太多，所以也无需太复杂。
static rlist_t __mmap_info_list__ = {&__mmap_info_list__ , &__mmap_info_list__} ;
static SRWLOCK __mmap_access_guard__ = SRWLOCK_INIT ;

bool mmap_insert(mmap_info_t * info) ;

bool mmap_delete(mmap_info_t *& info , void * addr , size_t size) ;
mmap_info_t * mmap_find(void * addr) ;

static inline bool mmap_inside(mmap_info_t * info , void * addr)
{
    return (((uintptr_t)info->map_addr < (uintptr_t)addr) && 
        ((uintptr_t)info->map_addr + info->use_size > (uintptr_t)addr)) ;
}

bool mmap_insert(mmap_info_t * info)
{
    rlist_init(&info->link) ;
    ::AcquireSRWLockExclusive(&__mmap_access_guard__) ;
    rlist_add_tail(&__mmap_info_list__ , &info->link) ;
    ::ReleaseSRWLockExclusive(&__mmap_access_guard__) ;

    return true ;
}

bool mmap_delete(mmap_info_t *& info , void * addr , size_t size) 
{
    info = NULL ;

    mmap_info_t * finfo = mmap_find(addr) ;
    if(finfo == NULL)
        return false ;

    bool result = true ;
    uintptr_t use_addr = (uintptr_t)finfo->use_addr ;
    uintptr_t del_addr = (uintptr_t)addr ;
    size_t use_size = finfo->use_size ;

    ::AcquireSRWLockExclusive(&__mmap_access_guard__) ;
    if(del_addr == use_addr)
    {
        if(size >= use_size)
        {
            rlist_del(NULL ,&finfo->link) ;
            info = finfo ;
        }
        else 
        {
            finfo->use_addr = (void *)(use_addr + size) ;
            finfo->use_size -= size ;
        }
    }
    else if(use_addr + use_size == del_addr + size)
    {
        finfo->use_size -= size ;        
    }
    else
        result = false ;

    ::ReleaseSRWLockExclusive(&__mmap_access_guard__) ;

    return result ;
}

mmap_info_t * mmap_find(void * addr)
{
    mmap_info_t * info = NULL , * cur = NULL;    
    ::AcquireSRWLockShared(&__mmap_access_guard__) ;
    rlist_t * next = __mmap_info_list__.next ;
    while(next != &__mmap_info_list__)
    {
        cur = (mmap_info_t *)next ;
        if((cur->use_addr == addr) || mmap_inside(cur , addr) == true)
        {
            info = cur ;
            break ;
        }

        next = next->next ;
    }
    
    ::ReleaseSRWLockShared(&__mmap_access_guard__) ;
    return info ;
}

void *mmap (void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    HANDLE hfile = INVALID_HANDLE_VALUE ;
    if(fd != -1)
    {
        //1、先定位到文件中对应的偏移量
        ::lseek(fd , offset , SEEK_SET) ;

        //fd是linux系统的句柄，需要转译成windows的句柄HANDLE
        hfile = (HANDLE)::_get_osfhandle(fd) ;
    }
    DWORD wprot = __mmap_prot_to_win(prot) ;
    DWORD wflags = __mmap_flag_to_win(flags) ;
    wprot |= wflags ;   
    if(wprot == 0)
        wprot = PAGE_READWRITE | SEC_COMMIT;
    HANDLE hmap = ::CreateFileMappingA(hfile , NULL , wprot , 0 , (DWORD)len , NULL) ;
    if(hmap == NULL)
    {
        DWORD errcode = ::GetLastError() ;
        return NULL ;
    }

    void * map_addr = ::MapViewOfFile(hmap , FILE_MAP_ALL_ACCESS , 0 , 0 , len) ;

    //2、保存信息
    size_t info_size = sizeof(mmap_info_t) ;
    mmap_info_t * info = (mmap_info_t *)::malloc(info_size) ;
    ::memset(info , 0 , info_size) ;
    rlist_init(&info->link) ;

    info->file_desc = fd ;
    info->prot = prot ;
    info->flags = flags ;
    info->len = len ;
    info->offset = offset ;

    info->expect_addr = addr ;
    info->file_handle = hfile ;
    info->map_handle = hmap ;
    info->map_addr = map_addr ;
    info->use_addr = map_addr ;
    info->use_size = len ;

    mmap_insert(info) ;    
    return map_addr ;
}

int munmap (void *addr, size_t len)
{
    mmap_info_t * info = NULL ;
    if(mmap_delete(info , addr , len) == false)
        return -1 ;

    if(info != NULL)
    {
        if(info->map_addr != NULL)
            ::UnmapViewOfFile(info->map_addr) ;

        if(info->map_handle != NULL)
            ::CloseHandle(info->map_handle) ;

        if(info->name != NULL)
            ::free(info->name) ;

        ::free(info) ;
    }
    return 0 ;
}

int mprotect (void *addr, size_t len, int prot)
{
    //::VirtualProtect() ;
    return 0 ;
}

int msync (void *addr, size_t len, int flags)
{
    if(::FlushViewOfFile(addr , len) == TRUE)
        return 0 ;
    else
        return -1 ;
}

int madvise (void *addr, size_t len, int advice)
{
    return -1 ;
}

int posix_madvise (void *addr, size_t len, int advice)
{
    return -1 ;
}

int mlock (const void *addr, size_t len)
{
    return (::VirtualLock((PVOID)addr , len) == TRUE ? 0 : -1);
}

int munlock (const void *addr, size_t len)
{
    return (::VirtualUnlock((PVOID)addr , len) == TRUE ? 0 : -1);
}

int mlockall (int flags)
{
    return 0 ;
}

int munlockall (void)
{
    return 0 ;
}

int shm_open (const char *name, int oflag , mode_t mode)
{
    return ::_open(name , oflag , mode) ;
}

int shm_unlink (const char *name)
{
    return ::_unlink(name) ;
}
