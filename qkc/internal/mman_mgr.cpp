
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

mmap_commits_t * mmap_info_find(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL)
		return NULL;

	rlist_t * next = info->commits.next;
	while (next != &info->commits)
	{
		mmap_commits_t * commit = (mmap_commits_t *)next;
		next = next->next;
		if (commit->start_addr == addr && commit->len == size)
			return commit;
	}
	return NULL;
}

bool mmap_info_commit(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL || size == 0)
		return false;

	if (mmap_info_inside(info, addr, size) == false)
		return false;

	//如果是全部提交，这是大部分情况，则直接表示在info上
	if (info->map_addr == addr && info->len == size)
	{
		if (info->wflags & MEM_COMMIT)
			return true;

		::AcquireSRWLockExclusive(&info->locker);
		if (::VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
		{
			DWORD errcode = ::GetLastError();
			::ReleaseSRWLockExclusive(&info->locker);
			return false;
		}
		info->wflags |= MEM_COMMIT;
		::ReleaseSRWLockExclusive(&info->locker);
		return true;
	}

	//分片提交
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
		mmap_heap_free(commit);
		return true;
	}

	if (::VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
	{
		DWORD errcode = ::GetLastError();
		::ReleaseSRWLockExclusive(&info->locker);

		mmap_heap_free(commit);
		return false;
	}
	commit->start_addr = addr;
	commit->len = size;
	rlist_add_tail(&info->commits, &commit->link);
	::ReleaseSRWLockExclusive(&info->locker);
	return true;
}

bool mmap_info_decommit(mmap_info_t * info, void * addr, size_t size)
{
	if (info == NULL || addr == NULL || size == 0)
		return false;

	if (mmap_info_inside(info, addr, size) == false)
		return false;

	bool result = false;
	::AcquireSRWLockExclusive(&info->locker);
	mmap_commits_t * commit = mmap_info_find(info, addr, size);
	if (commit == NULL)
	{
		::VirtualFree(addr, size, MEM_DECOMMIT);
		info->wflags &= MEM_COMMIT;
		result = true;
	}
	else
	{
		rlist_del(&info->commits, &commit->link);
		::VirtualFree(addr, size, MEM_DECOMMIT);
		mmap_heap_free(commit);
		result = true;
	}
	::ReleaseSRWLockExclusive(&info->locker);
	return result ;
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

		//这些标识还蛮多的，后面再处理。
	return 0;
}

