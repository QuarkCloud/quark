
#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>
#include "internal/rlist.h"
#include "internal/mman_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>



void * mmap_mem(mmap_info_t * info , void * addr, size_t len, int prot, int flags, int fd, off_t offset);
void * mmap_file(mmap_info_t * info, void * addr, size_t len, int prot, int flags, int fd, off_t offset);

void *mmap (void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	mmap_info_t * info = NULL;
	if (addr != NULL)
		info = mmap_mgr_find(NULL , addr);

	if (fd == -1 || (flags & MAP_ANON))
		return mmap_mem(info , addr, len, prot, flags, fd, offset);

	if (addr != NULL)
	{
		//可能没有标识，只是简单的释放
		if (info != NULL)
		{
			if (info->func_type == MMAP_FUNC_TYPE_ALLOC)
				return mmap_mem(info, addr, len, prot, flags, fd, offset);
			else if (info->func_type == MMAP_FUNC_TYPE_FILE)
				return mmap_file(info, addr, len, prot, flags, fd, offset);
			else
				return NULL;
		}		
	}

	return mmap_file(NULL, addr, len, prot, flags, fd, offset);
/**
	//1、找到对应的句柄
    HANDLE hfile = INVALID_HANDLE_VALUE ;
    if(fd != -1)
    {
        //1、先定位到文件中对应的偏移量
        ::lseek(fd , offset , SEEK_SET) ;

        //fd是linux系统的句柄，需要转译成windows的句柄HANDLE
        hfile = (HANDLE)::_get_osfhandle(fd) ;
		return NULL;
    }
    DWORD wprot = __mmap_prot_to_win(prot) ;
    DWORD wflags = __mmap_flag_to_win(flags) ;
    wprot |= wflags ;   
    if(wprot == 0)
        wprot = PAGE_READWRITE | SEC_COMMIT;

	//2、映射内存
	HANDLE hmap = INVALID_HANDLE_VALUE;
	void * map_addr = NULL;
	mmap_info_t * info = NULL;
	if (addr != NULL)
		info = mmap_find(addr);

	mmap_func_type func_type = MMAP_FUNC_TYPE_NONE;

	if ((fd != -1) && (hfile != INVALID_HANDLE_VALUE) && (hfile != NULL))
	{
		HANDLE hmap = ::CreateFileMappingA(hfile, NULL, wprot, 0, (DWORD)len, NULL);
		if (hmap == NULL)
		{
			DWORD errcode = ::GetLastError();
			return NULL;
		}

		void * map_addr = ::MapViewOfFile(hmap, FILE_MAP_ALL_ACCESS, 0, 0, len);
		if (map_addr == NULL)
		{
			DWORD errcode = ::GetLastError();
			::CloseHandle(hmap);
			return NULL;
		}

		func_type = MMAP_FUNC_TYPE_FILE;
	}
	else
	{
		if ((prot & PROT_READ) != 0 && (prot & PROT_WRITE) != 0)
		{
			DWORD old_wflags = 0;
			if (info != NULL)
				old_wflags = info->wflags;
			if((old_wflags & MEM_COMMIT) == 0)
				wflags |= MEM_COMMIT;

			map_addr = VirtualAlloc(addr, len, MEM_RESERVE | wflags, PAGE_READWRITE);
			if (map_addr == NULL)
			{
				DWORD errcode = ::GetLastError();
				return NULL;
			}
		}
		else if (prot == PROT_NONE)
		{
			if (addr != NULL)
			{
				::VirtualFree(addr, len, MEM_DECOMMIT);
			}
			if (info != NULL)
			{
				return NULL;
			}
		}

		func_type = MMAP_FUNC_TYPE_ALLOC;
	}

    //3、保存信息
	if (info == NULL)
	{
		size_t info_size = sizeof(mmap_info_t);
		info = (mmap_info_t *)::malloc(info_size);
		::memset(info, 0, info_size);
		rlist_init(&info->link);

		info->file_desc = fd;
		info->prot = prot;
		info->flags = flags;
		info->wprot = wprot;
		info->wflags = wflags;
		info->len = len;
		info->offset = offset;

		info->expect_addr = addr;
		info->file_handle = hfile;
		info->map_handle = hmap;
		info->map_addr = map_addr;
		info->use_addr = map_addr;
		info->use_size = len;
		info->func_type = func_type;

		mmap_insert(info);
	}
    return map_addr ;
*/
}

int mmap_unmem(mmap_info_t * info, void * addr, size_t len);
int mmap_unfile(mmap_info_t * info, void * addr, size_t len);

int munmap (void *addr, size_t len)
{
	//和mmap一样，munmap也会有分片释放的问题，甚至还有合并的问题
    mmap_info_t * info = mmap_mgr_find(NULL , addr) ;
	if (info == NULL)
		return -1;

	if (info->func_type == MMAP_FUNC_TYPE_ALLOC)
		return mmap_unmem(info, addr, len);
	else if (info->func_type == MMAP_FUNC_TYPE_FILE)
		return mmap_unfile(info, addr, len);
	else
		return -1;
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

void * mmap_mem(mmap_info_t * info, void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	DWORD wflags = 0;
	void * map_addr = NULL;

	if (info == NULL)
	{
		//还没有分配
		wflags = MEM_RESERVE;
		if ((prot & PROT_WRITE) && (prot & PROT_READ))
			wflags |= MEM_COMMIT;

		map_addr = ::VirtualAlloc(addr, len, wflags , PAGE_READWRITE);
		::printf("mmap_mem : VritualAlloc addr = %p , map_addr = %p , len = %x wflags = %s , prot = %d , flags = %d \n" , 
			addr , map_addr , len , __mmap_str_wflags(wflags) , prot , flags );
		if (map_addr == NULL)
		{
			DWORD errcode = ::GetLastError();
			return NULL;
		}

		size_t info_size = sizeof(mmap_info_t);
		mmap_info_t * new_info = NULL;	
		if (mmap_info_alloc(new_info) == false)
			return NULL;

		info = new_info;

		info->prot = prot;
		info->flags = flags;
		info->wflags = wflags;

		info->len = len;
		info->offset = offset;

		info->expect_addr = addr;
		info->file_handle = INVALID_HANDLE_VALUE;
		info->map_handle = INVALID_HANDLE_VALUE ;
		info->map_addr = map_addr;
		info->use_addr = map_addr;
		info->use_size = len;
		info->func_type = MMAP_FUNC_TYPE_ALLOC;

		mmap_mgr_insert(NULL , info);

		return map_addr;
	}
	else
	{
		/**
		if (info->map_addr != addr)
		{
			errno = EINVAL;
			return NULL;
		}
		*/

		//已经被分配了，那么可能是后续的操作，提交或者释放
		if (prot == PROT_NONE)
		{
			//内存不可访问
			bool result = mmap_info_decommit(info, addr, len);
			::printf("mmap_mem mmap_info_decommit info = %p , addr = %p , len = %x , result = %s \n", 
				info , addr , len , result?"TRUE":"FALSE");
			info->prot = prot;
			info->flags = flags;
			return addr;
		}
		else if ((prot & PROT_READ) && (prot & PROT_WRITE))
		{
			if (!(info->wflags & MEM_COMMIT))
			{
				bool result = mmap_info_commit(info, addr, len);
				::printf("mmap_mem mmap_info_commit info=%p , addr = %p , map_addr = %p , "
					"len = %x wflags = %s , prot = %x , flags = %x result=%s\n",
					info , addr, info->map_addr, len, __mmap_str_wflags(info->wflags), prot, flags , result?"true":"false");
				if (result == false)
					return NULL;
			}
			return addr;
		}
	}
	return NULL;
}

void * mmap_file(mmap_info_t * info, void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return NULL;
}

int mmap_unmem_head(mmap_info_t * info, void * addr, size_t len)
{
	//释放头部
	::VirtualFree(addr, len, MEM_RELEASE);
	info->map_addr = (void *)((uintptr_t)addr + len);
	info->len -= len;
	return 0;
}

int mmap_unmem_tail(mmap_info_t * info, void * addr, size_t len)
{
	//else if ((uintptr_t)info->map_addr + info->len == (uintptr_t)addr + len)
	{
		//释放尾部
		::VirtualFree((void *)((uintptr_t)addr + len), len, MEM_RELEASE);
		info->len -= len;
		return 0;
	}
}

int mmap_unmem_single(mmap_info_t * info, void * addr, size_t len)
{
	if (mmap_mgr_delete(NULL, info) == false)
		return -1;

	if (info->map_addr != NULL)
	{
		::VirtualFree(info->map_addr, 0, MEM_RELEASE);
		::printf("VirtualFree map_addr =%p , len = %x \n", info->map_addr , len);
		info->map_addr = NULL;
	}

	if (info->name != NULL)
		::free(info->name);

	mmap_info_free(info);

	return 0;
}

int mmap_unmem_multiple(mmap_info_t * info, void * addr, size_t len)
{
	void * ptr = addr;
	mmap_info_t * node = info;
	size_t size = len;
	rlist_t infos;
	rlist_init(&infos);

	while (node->map_addr == ptr && node->len <= size)
	{
		mmap_mgr_delete(NULL, node);
		rlist_add_tail(&infos, &node->link);

		ptr = (void *)((uintptr_t)ptr + node->len);
		size -= node->len;
		if (size == 0)
			break;

		if ((node = mmap_mgr_find(NULL, ptr)) == NULL)
			return -1;
	}

	if (size != 0)
	{
		//没有全部对齐，则重新塞回去
		while (rlist_empty(&infos) == false)
		{
			node = (mmap_info_t *)infos.next;
			rlist_del(NULL, &node->link);

			mmap_mgr_insert(NULL, node);
		}
		return -1;
	}

	//全部释放
	int result = 0;
	while (rlist_empty(&infos) == false)
	{
		node = (mmap_info_t *)infos.next;
		rlist_del(&infos, &node->link);
		if (mmap_unmem_single(node, node->map_addr, node->len) != 0)
		{
			mmap_mgr_insert(NULL, node);
			result = -1;
		}
	}

	return result;
}

int mmap_unmem(mmap_info_t * info, void * addr, size_t len)
{
	if (info->map_addr == addr)
	{
		if (info->len == len || len == 0)
		{
			//整块释放
			return mmap_unmem_single(info, addr, len);
		}
		else if (info->len > len)
		{
			//分片释放
			return mmap_unmem_head(info, addr, len);
		}
		else
			return mmap_unmem_multiple(info, addr, len);
	}
	else if ((uintptr_t)info->map_addr + info->len == (uintptr_t)addr + len)
	{
		//分片释放
		return mmap_unmem_tail(info, addr, len);
	}
	return -1;
}

int mmap_unfile(mmap_info_t * info, void * addr, size_t len)
{

	if (info->map_addr == addr && (info->len == len || len == 0))
	{
		if (mmap_mgr_delete(NULL, info) == false)
			return -1;

		if (info->func_type == MMAP_FUNC_TYPE_FILE)
		{
			if (info->map_addr != NULL)
				::UnmapViewOfFile(info->map_addr);

			if (info->map_handle != NULL)
				::CloseHandle(info->map_handle);
		}

		if (info->name != NULL)
			::free(info->name);

		mmap_info_free(info);
		return 0;
	}
	else
	{
		//分片释放

		//合并释放

	}
	return -1;
}

