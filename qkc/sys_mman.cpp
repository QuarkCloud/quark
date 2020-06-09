
#include <sys/mman.h>
#include <windows.h>
#include "wimpl/MMapMgr.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "wobjs/CrtLinker.h"
#include "wobjs/ObjMgr.h"


void * mmap_mem(void * addr, size_t len, int prot, int flags, int fd, off_t offset);
void * mmap_file(void * addr, size_t len, int prot, int flags, int fd, off_t offset);
void * mmap_shm(void * addr, size_t len, int prot, int flags, int fd, off_t offset);

void *mmap(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	//1、先根据输入参数，区分不同的底层映射机制
	if (fd == -1 || (flags & MAP_ANON))
		return mmap_mem(addr, len, prot, flags, fd, offset);

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(fd);
	if (obj->IsFile())
		return mmap_file(addr, len, prot, flags, fd, offset);
	else if (obj->IsShareMemory())
		return mmap_shm(addr, len, prot, flags, fd, offset);
	return NULL;
}

int munmap (void *addr, size_t len)
{
	if (addr == NULL)
		return - 1;

	qkc::MPageInfo * info = NULL;
	if (qkc::MPageMgr::Singleton().Delete(addr, info) == false || info == NULL)
		return -1;

	delete info;
	return 0;
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

void * mmap_mem(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	DWORD wflags = qkc::MPageInfo::FlagToWin(prot, flags);

	qkc::MMapInfo * info = NULL;
	if (addr != NULL)
	{
		//存在两种可能，一种是已经预留了地址空间的后续操作，另外一种是申请指定地址的内存
		info = (qkc::MMapInfo *)qkc::MPageMgr::Singleton().Find(addr);
		if (info != NULL)
		{
			if (wflags & MEM_COMMIT)
			{
				if (info->Commit(addr, len) == true)
					return addr;
			}
			else if (wflags & MEM_DECOMMIT)
			{
				if (info->Decommit(addr, len) == true)
					return addr;
			}
			return NULL;
 		}
	}

	info = new qkc::MMapInfo();
	if (info->Init(addr, len, prot, flags) == true)
	{
		if (qkc::MPageMgr::Singleton().Insert(info) == true)
			return info->Addr();
	}
	delete info;
	return NULL;
}

void * mmap_file(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return NULL;
}

void * mmap_shm(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return NULL;
}

