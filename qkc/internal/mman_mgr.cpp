
#include "mman_mgr.h"

static inline void * mmap_heap_alloc(size_t size)
{
	return ::HeapAlloc(::GetProcessHeap(), 0x00000008, size);
}

static inline void mmap_heap_free(void * addr)
{
	::HeapFree(::GetProcessHeap(), 0, addr);
}


mmap_mgr_t * mmap_mgr_default()
{
	static mmap_mgr_t __mmap_mgr_default__ = { SRWLOCK_INIT , {&__mmap_mgr_default__.infos , &__mmap_mgr_default__.infos} };
	
	return &__mmap_mgr_default__;
}

bool mmap_mgr_insert(mmap_mgr_t * mgr, mmap_info_t * info)
{
	if (info == NULL)
		return false;

	if (mgr == NULL)
		mgr = mmap_mgr_default();

	::AcquireSRWLockExclusive(&mgr->guard);
	rlist_init(&info->link);
	rlist_add_tail(&mgr->infos, &info->link);
	::ReleaseSRWLockExclusive(&mgr->guard);
	return true;
}

bool mmap_mgr_delete(mmap_mgr_t * mgr, mmap_info_t *info)
{
	if (info == NULL)
		return false;

	if (mgr == NULL)
		mgr = mmap_mgr_default();

	::AcquireSRWLockExclusive(&mgr->guard);

	rlist_del(&mgr->infos, &info->link);

	::ReleaseSRWLockExclusive(&mgr->guard);

	return true;
}

mmap_info_t * mmap_mgr_find(mmap_mgr_t * mgr, void * addr)
{
	if (addr == NULL)
		return NULL;
	if (mgr == NULL)
		mgr = mmap_mgr_default();

	mmap_info_t * info = NULL, *cur = NULL;
	::AcquireSRWLockShared(&mgr->guard);
	rlist_t * next = mgr->infos.next;
	while (next != &mgr->infos)
	{
		cur = (mmap_info_t *)next;
		if ((cur->use_addr == addr) || mmap_info_inside(cur, addr , 0) == true)
		{
			info = cur;
			break;
		}

		next = next->next;
	}

	::ReleaseSRWLockShared(&mgr->guard);
	return info;
}

bool mmap_mgr_validate(mmap_mgr_t * mgr, void * addr, size_t size)
{
	if (mgr == NULL)
		mgr = mmap_mgr_default();

	void * ptr = addr;
	size_t max_size = size;
	mmap_info_t * info = NULL;

	while ((info = mmap_mgr_find(mgr, ptr)) != NULL)
	{
		size_t move_size = ((uintptr_t)info->map_addr + info->len) - (uintptr_t)ptr;
		if (move_size >= max_size)
			return true;

		max_size -= move_size;
		ptr = (void *)((uintptr_t)ptr + move_size);
	}
	return true;
}

bool mmap_info_alloc(mmap_info_t *& info)
{
	info = (mmap_info_t *)mmap_heap_alloc(sizeof(mmap_info_t));
	if (info == NULL)
		return false;

	rlist_init(&info->link);
	info->locker = SRWLOCK_INIT;

	info->fd = -1;
	info->prot = 0;
	info->flags = 0;
	info->wflags = 0;

	info->len = 0;
	info->offset = 0;
	info->expect_addr = NULL;

	rlist_init(&info->commits);

	info->file_handle = INVALID_HANDLE_VALUE;
	info->map_handle = INVALID_HANDLE_VALUE;
	info->map_addr = NULL;

	info->use_addr = NULL;
	info->use_size = 0;
	info->func_type = MMAP_FUNC_TYPE_NONE;
	info->name = NULL;

	return true;
}

bool mmap_info_free(mmap_info_t * info)
{
	if (info == NULL)
		return false;

	::AcquireSRWLockExclusive(&info->locker);

	while (rlist_empty(&info->commits) == false)
	{
		rlist_t * next = info->commits.next;
		rlist_del(&info->commits, next);
		mmap_heap_free(next);
	}

	::ReleaseSRWLockExclusive(&info->locker);
	mmap_heap_free(info);
	return true;
}

//ֻҪ���Ѿ��ύ���ڴ�ҳ�ڣ�����
mmap_commits_t * mmap_info_find(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL)
		return NULL;

	rlist_t * next = info->commits.next;
	while (next != &info->commits)
	{
		mmap_commits_t * commit = (mmap_commits_t *)next;
		next = next->next;
		if ((commit->start_addr <= addr) && 
			((uintptr_t)commit->start_addr + commit->len >= (uintptr_t)addr + (size?size:1)))
			return commit;
	}
	return NULL;
}

bool mmap_info_commit(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL || size == 0)
		return false;
	/**
		2019-03-20
		һ�������޵е�Ӧ�ó������ύ���Կ���mmap���飬�����ټ�ʶ�ˣ�����֮ǰ̫�й��о��ˡ�
	*/
	//if (mmap_info_inside(info, addr, size) == false)
	//	return false;

	//�����ȫ���ύ�����Ǵ󲿷��������ֱ�ӱ�ʾ��info��
	if (info->map_addr == addr && info->len == size)
	{
		if (info->wflags & MEM_COMMIT)
		{
			//::printf("whole info = %p addr = %p , size = %x had commit \n" , info , addr , size);
			return true;
		}

		::AcquireSRWLockExclusive(&info->locker);
		if (::VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
		{
			DWORD errcode = ::GetLastError();
			//::printf("virtual alloc commit failed errcode = %u, addr=%p size=%x \n" , errcode , addr , size);
			::ReleaseSRWLockExclusive(&info->locker);
			return false;
		}
		//::printf("virtual alloc commit succeed , addr=%p size=%x \n", addr, size);
		info->wflags |= MEM_COMMIT;
		::ReleaseSRWLockExclusive(&info->locker);
		return true;
	}
	else if ((uintptr_t)info->map_addr > (uintptr_t)addr)
	{
		//��ַ����������
		return false;
	}

	if ((uintptr_t)info->map_addr + info->len >= (uintptr_t)addr + size)
	{
		//Ƭ�ڷ�Ƭ�ύ
		mmap_commits_t * commit = (mmap_commits_t *)mmap_heap_alloc(sizeof(mmap_commits_t));
		if (commit == NULL)
		{
			errno = ENOMEM;
			return false;
		}
		rlist_init(&commit->link);

		::AcquireSRWLockExclusive(&info->locker);

		if (mmap_info_find(info, addr, size) != NULL)
		{
			::ReleaseSRWLockExclusive(&info->locker);
			//::printf("piece info = %p addr = %p , size = %x had commit \n", info , addr, size);
			mmap_heap_free(commit);
			return true;
		}

		if (::VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
		{
			DWORD errcode = ::GetLastError();
			//::printf("virtual alloc commit failed errcode = %u, addr=%p size=%x \n", errcode, addr, size);
			::ReleaseSRWLockExclusive(&info->locker);

			mmap_heap_free(commit);
			return false;
		}
		//::printf("virtual alloc commit succeed , addr=%p size=%x \n", addr, size);
		commit->start_addr = addr;
		commit->len = size;
		rlist_add_tail(&info->commits, &commit->link);
		::ReleaseSRWLockExclusive(&info->locker);
		return true;
	}

	//����ύ�����˱���mmap���⣬���漰�������mmap��
	//1���ȼ���Ƿ���mgr�ڲ������ڴ��
	if (mmap_mgr_validate(NULL, addr, size) == false)
		return false;
	//2������ύ
	mmap_info_t * node = NULL;
	void * ptr = addr;
	size_t max_size = size;
	while ((node = mmap_mgr_find(NULL, ptr)) != NULL)
	{
		size_t move_size = ((uintptr_t)node->map_addr + node->len) - (uintptr_t)ptr;
		if (max_size <= move_size)
			return mmap_info_commit(node, ptr, max_size);
		else
			mmap_info_commit(node, ptr, move_size);

		max_size -= move_size;
		ptr = (void *)((uintptr_t)ptr + move_size);
	}

	return false;	
}

bool mmap_info_decommit(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL || size == 0)
	{
		//::printf("decommit failed , info=%p addr = %p size = %x \n" , info , addr , size);
		return false;
	}

	if (mmap_info_inside(info, addr, 0) == false)
	{
		//::printf("decommit failed for addr not inside , info=%p addr = %p size = %x \n", info, addr, size);
		return false;
	}

	bool result = false;
	::AcquireSRWLockExclusive(&info->locker);
	mmap_commits_t * commit = mmap_info_find(info, addr, 0);
	if (commit == NULL)
	{
		//�������ǲ�������decommit��������������ĳ���
		if ((info->wflags & MEM_COMMIT) && (info->map_addr == addr) && (info->len == size))
		{
			::VirtualFree(addr, size, MEM_DECOMMIT);
			info->wflags &= ~MEM_COMMIT;
			result = true;
			//::printf("decommit whole info = %p , addr = %p , len = %x \n" , info , addr , size);
		}
		else
		{
			//::printf("decommit failed , info=%p wflags = %s map_addr = %p , addr = %p info->len = %x size = %x \n", 
			//	info, ::__mmap_str_wflags(info->wflags) , info->map_addr , addr, info->len , size);
		}
		::ReleaseSRWLockExclusive(&info->locker);
		return result;
	}
	else if (((uintptr_t)commit->start_addr <= (uintptr_t)addr) && 
		((uintptr_t)commit->start_addr + commit->len) >= (uintptr_t)addr + (size?size:1))
	{
		//���ų���Ƭdecommit�����
		if (commit->start_addr == addr)
		{
			size_t desize = size;
			if (desize == 0)
				desize = commit->len;

			if (::VirtualFree(addr, desize, MEM_DECOMMIT) == FALSE)
			{
				//::printf("virtual free whole commit failed , info=%p , addr=%p , size = %x \n", info, addr, desize);
				result = false;
			}
			else
				result = true;

			if (desize == commit->len)
			{
				//��Ƭ�ͷ�
				rlist_del(&info->commits, &commit->link);
				mmap_heap_free(commit);
			}
		}
		else if((size == 0) ||((uintptr_t)commit->start_addr + commit->len) == (uintptr_t)addr + size)
		{
			//β���ͷ�
			size_t desize = size;
			if (desize == 0)
				desize = ((uintptr_t)commit->start_addr + commit->len) - (uintptr_t)addr;

			if (::VirtualFree(addr, desize, MEM_DECOMMIT) == FALSE)
			{
				//::printf("virtual free whole commit failed , info=%p , addr=%p , size = %x \n", info, addr, desize);
				result = false;
			}
			else
			{
				commit->len -= desize;
				result = true;
			}
		}
		else
		{
			//�ж��ͷ�
			if (::VirtualFree(addr, size, MEM_DECOMMIT) == FALSE)
			{
				//::printf("virtual free whole commit failed , info=%p , addr=%p , size = %x \n", info, addr, size);
				result = false;
			}
			else
			{
				size_t new_size = sizeof(mmap_commits_t);
				mmap_commits_t * new_commit = (mmap_commits_t *)::HeapAlloc(::GetProcessHeap(), 0, new_size);
				if (new_commit != NULL)
				{
					rlist_init(&new_commit->link);

					new_commit->start_addr = (void *)((uintptr_t)addr + size);
					new_commit->len = ((uintptr_t)commit->start_addr + commit->len) - (uintptr_t)addr - size;

					rlist_add_tail(&info->commits, &new_commit->link);

					commit->len = (uintptr_t)addr - (uintptr_t)commit->start_addr;
				}
			}
		}

		::ReleaseSRWLockExclusive(&info->locker);
		return result;
	}
	else
	{
		//���commit���ϲ�decommit
		size_t max_size = size;
		if (size == 0)
			max_size = ((uintptr_t)info->map_addr + info->len) - (uintptr_t)addr;
		void * ptr = addr;
		result = true;
		while (commit != NULL && max_size > 0)
		{
			size_t desize = commit->len;
			if (desize > max_size)
			{
				desize = max_size;
				commit->start_addr = (void *)((uintptr_t)ptr + desize);
				commit->len -= desize;
			}
			else
			{
				rlist_del(&info->commits, &commit->link);
				mmap_heap_free(commit);
			}
			if (::VirtualFree(ptr, desize, MEM_DECOMMIT) == FALSE)
			{
				//::printf("virtual free piece failed , info=%p , addr=%p , size = %x \n", info, ptr, desize);
				result = false;
				break;
			}
			//::printf("virtual free piece succeed , info=%p , addr=%p , size = %x \n", info, ptr, desize);

			max_size -= desize;
			ptr = (void *)((uintptr_t)ptr + desize);

			commit = mmap_info_find(info, ptr, 0);
		}
		::ReleaseSRWLockExclusive(&info->locker);


		if (result == true && max_size > 0)
		{
			//��ʣ�µģ����������ⲿmmap_info_t
			//::printf("decommit left size = %x , info=%p map_addr%p size=%x\n", max_size, info, info->map_addr, info->len);
			mmap_info_t * node = NULL;
			while ((node = mmap_mgr_find(NULL, ptr)) != NULL && max_size > 0)
			{
				if (node->map_addr != ptr)
				{
					result = false;
					break;
				}
				size_t desize = node->len;
				if (desize > max_size)
					desize = max_size;
				if (mmap_info_decommit(node, ptr, desize) == false)
				{
					result = false;
					break;
				}

				ptr = (void *)((uintptr_t)ptr + desize);
				max_size -= desize;
			}
		}

		return result;
	}
	return false;
}

DWORD __mmap_prot_to_win(int prot)
{
	DWORD wp = 0;
	prot &= 0x7;
	if (prot == PROT_READ)
		wp = PAGE_READONLY;
	else if (prot == PROT_WRITE)
		wp = PAGE_WRITECOPY;
	else if (prot == PROT_EXEC)
		wp = PAGE_EXECUTE_READ;
	else if (prot == (PROT_READ | PROT_WRITE))
		wp = PAGE_READWRITE;
	else if (prot == (PROT_READ | PROT_EXEC))
		wp = PAGE_EXECUTE_READ;
	else if (prot == (PROT_WRITE | PROT_EXEC))
		wp = PAGE_EXECUTE_WRITECOPY;
	else if (prot == (PROT_READ | PROT_WRITE | PROT_EXEC))
		wp = PAGE_EXECUTE_READWRITE;

	return wp;
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

		//��Щ��ʶ������ģ������ٴ���
	return 0;
}

const char * __mmap_str_wflags(DWORD wflags)
{
	static const char * __str_wflags__[] = {
		"MEM_COMMIT | MEM_RESERVE" ,
		"MEM_COMMIT" , "MEM_RESERVE" ,
		"NOFLAGS"
	};

	if ((wflags & MEM_COMMIT) && (wflags & MEM_RESERVE))
		return __str_wflags__[0];
	else if (wflags & MEM_COMMIT)
		return __str_wflags__[1];
	else if (wflags & MEM_RESERVE)
		return __str_wflags__[2];
	else
		return __str_wflags__[3];
}

