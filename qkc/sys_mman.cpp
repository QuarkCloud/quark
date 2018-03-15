
#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>
#include "inner/rlist.h"
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

    char * name ;
} mmap_info_t ;

//偷个懒，用链表管理。因为mmap本身就不会太多，所以也无需太复杂。
static rlist_t __mmap_info_list__ = {&__mmap_info_list__ , &__mmap_info_list__} ;
static SRWLOCK __mmap_access_guard__ = SRWLOCK_INIT ;

bool mmap_insert(mmap_info_t * info) ;
mmap_info_t * mmap_delete(void * map_addr) ;
mmap_info_t * mmap_find(void * map_addr) ;

bool mmap_insert(mmap_info_t * info)
{
    rlist_init(&info->link) ;
    ::AcquireSRWLockExclusive(&__mmap_access_guard__) ;
    rlist_add_tail(&__mmap_info_list__ , &info->link) ;
    ::ReleaseSRWLockExclusive(&__mmap_access_guard__) ;

    return true ;
}

mmap_info_t * mmap_delete(void * map_addr) 
{
    mmap_info_t * info = mmap_find(map_addr) ;
    if(info != NULL)
    {
        ::AcquireSRWLockExclusive(&__mmap_access_guard__) ;
        rlist_init(&info->link) ;
        ::ReleaseSRWLockExclusive(&__mmap_access_guard__) ;
    }

    return info ;
}

mmap_info_t * mmap_find(void * map_addr)
{
    mmap_info_t * info = NULL , * cur = NULL;    
    ::AcquireSRWLockShared(&__mmap_access_guard__) ;
    rlist_t * next = __mmap_info_list__.next ;
    while(next != &__mmap_info_list__)
    {
        cur = (mmap_info_t *)next ;
        if(cur->map_addr == map_addr)
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
    //1、先定位到文件中对应的偏移量
    ::lseek(fd , offset , SEEK_SET) ;

    //fd是linux系统的句柄，需要转译成windows的句柄HANDLE
    HANDLE hfile = (HANDLE)::_get_osfhandle(fd) ;
    DWORD wprot = __mmap_prot_to_win(prot) ;
    DWORD wflags = __mmap_flag_to_win(flags) ;
    wprot |= wflags ;
    HANDLE hmap = ::CreateFileMappingA(hfile , NULL , wprot , 0 , (DWORD)len , NULL) ;

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

    mmap_insert(info) ;    
    return map_addr ;
}

int munmap (void *addr, size_t len)
{
    mmap_info_t * info = mmap_delete(addr) ;
    if(info == NULL)
        return -1 ;

    if(info->map_addr != NULL)
        ::UnmapViewOfFile(addr) ;

    if(info->map_handle != NULL)
        ::CloseHandle(info->map_handle) ;

    if(info->name != NULL)
        ::free(info->name) ;

    ::free(info) ;
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
