
#include "wimpl/MMapMgr.h"
#include "wobjs/ErrorMap.h"
#include "wobjs/ObjMgr.h"
#include <errno.h>

namespace qkc {

	MRange::MRange()
	{
		addr_ = NULL;
		size_ = 0;
	}
	MRange::MRange(void * addr, size_t size)
	{
		Assign(addr, size);
	}
	MRange::MRange(const MRange& range)
	{
		Assign(range);
	}
	MRange::~MRange()
	{
		//
	}
	MRange& MRange::operator=(const MRange& range)
	{
		Assign(range);
		return (*this);
	}

	bool MRange::IsInside(const void * addr, size_t size) const
	{
		if ((uintptr_t)addr < (uintptr_t)addr_)
			return false;

		if ((uintptr_t)addr_ + size_ < (uintptr_t)addr + (size == 0) ? 1 : size)
			return false;

		return true;
	}

	void MRange::Assign(void * addr, size_t size)
	{
		addr_ = addr;
		size_ = size;
	}
	void MRange::Assign(const MRange& range)
	{
		addr_ = (void *)range.Addr();
		size_ = range.Size();
	}

	MPageCommit::MPageCommit()
	{
		node_ = NULL;
	}

	MPageCommit::MPageCommit(const MPageCommit& commit)
	{
		node_ = commit.node_;
	}

	MPageCommit::MPageCommit(const AddrNode * node)
	{
		node_ = (AddrNode *)node;
	}

	MPageCommit::~MPageCommit()
	{
		//
	}

	MPageCommit& MPageCommit::operator=(const MPageCommit& commit)
	{
		node_ = commit.node_;
		return (*this);
	}

	MPageCommit& MPageCommit::operator=(const AddrNode * node)
	{
		node_ = (AddrNode *)node;
		return (*this);
	}

	const void * MPageCommit::Addr() const
	{
		if (node_ == NULL)
			return NULL;
		else
			return node_->Addr;
	}
	void * MPageCommit::Addr()
	{
		if (node_ == NULL)
			return NULL;
		else
			return node_->Addr;
	}
	void MPageCommit::Addr(const void * addr)
	{
		if (node_ != NULL)
			node_->Addr = (void *)addr;
	}
	size_t MPageCommit::Size() const
	{
		if (node_ != NULL)
			return (size_t)node_->Data;
		return 0;
	}
	void MPageCommit::Size(size_t size)
	{
		if (node_ != NULL)
			node_->Data = size;
	}
	bool MPageCommit::IsInside(const void * addr, size_t size) const
	{
		if ((uintptr_t)addr < (uintptr_t)Addr())
			return false;

		if ((uintptr_t)Addr() + Size() < (uintptr_t)addr + (size == 0) ? 1 : size)
			return false;

		return true;
	}

	MPageInfo::MPageInfo()
	{
		fd_ = -1;
		prot_ = 0;
		flags_ = 0;
		wflags_ = 0;
		offset_ = 0;
		type_ = kMMapTypeNone;
	}

	MPageInfo::~MPageInfo()
	{
		Final();
	}


	MPageCommit MPageInfo::Find(const void * addr, size_t size) const
	{
		RWLocker::ScopedSharedLock locker(guard_);
		return Search(addr, size);
	}

	bool MPageInfo::Commit(void * addr, size_t size)
	{
		if (addr == NULL)
			return false;

		RWLocker::ScopedLock locker(guard_);
		if (Addr() == addr && size == Size())
			return CommitAllPages();
		else if ((uintptr_t)Addr() > (uintptr_t)addr)
			return false;

		if ((uintptr_t)Addr() + Size() >= (uintptr_t)addr + size)
			return CommitPiece(addr, size);

		/**跨界提交，除了本地mmap块外，还涉及到另外的mmap块，不在本地执行，由具备更多信息的管理器执行*/
		return false;
	}

	bool MPageInfo::Decommit(void * addr, size_t size)
	{
		if (addr == NULL)
			return false;
		if (IsInside(addr, size) == false)
			return false;

		bool result = false;
		RWLocker::ScopedLock locker(guard_);
		MPageCommit commit = Search(addr, 0);
		if (commit.Empty() == true)
		{
			//地址在区域内，但没有找到对应的提交页。先试试是不是整个decommit，这是最经常发生的场景
			if ((wflags_ & MEM_COMMIT) && (Addr() == addr) && (Size() == size))
				return DecommitAllPages();
			else
				return false;
		}

		if (commit.IsInside(addr, size) == true)
			return DecommitSinglePiece(commit, addr, size);

		return DecommitMultiPiece(commit, addr, size);
	}
	void MPageInfo::Final()
	{
		//
	}

	DWORD MPageInfo::ProtToWin(int prot)
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

	DWORD MPageInfo::FlagToWin(int prot, int flag)
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

		/**
			2020-04-24
			先处理一些紧要的标记
		*/
		DWORD wflags = MEM_RESERVE;
		if (prot & PROT_NONE)
		{
			wflags = MEM_DECOMMIT;
		}
		else if ((prot & PROT_READ) && (prot & PROT_WRITE))
		{
			wflags |= MEM_COMMIT;
		}

		return wflags;
	}
	const char * MPageInfo::StrWinFlag(DWORD wflags)
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

	bool MPageInfo::CommitAllPages()
	{
		return true;
	}

	bool MPageInfo::CommitPiece(void * addr, size_t size)
	{
		return true;
	}

	MPageCommit MPageInfo::Search(const void * addr, size_t size) const
	{
		const AddrNode * cur = commits_.Root();
		while (cur != NULL)
		{
			MPageCommit commit = cur;
			if(commit.IsInside(addr , size) == true)
				return commit;

			if ((uintptr_t)cur->Addr > (uintptr_t)addr)
				cur = (const AddrNode *)cur->Right;
			else
				cur = (const AddrNode *)cur->Left;
		}
		return MPageCommit();
	}

	bool MPageInfo::DecommitAllPages()
	{
		return true;
	}

	bool MPageInfo::DecommitSinglePiece(MPageCommit& commit, void * addr, size_t size)
	{
		return true;
	}

	bool MPageInfo::DecommitMultiPiece(MPageCommit& commit, void * addr, size_t size)
	{
		return true;
	}

	MPageMgr::MPageMgr(MemMgr * mmgr)
	{
		mmgr_ = NULL;
		Init(mmgr);
	}

	MPageMgr::~MPageMgr()
	{
		Final();
	}

	bool MPageMgr::Init(MemMgr * mmgr)
	{
		RWLocker::ScopedLock locker(guard_);
		if (mmgr == NULL)
			mmgr = &MemMgr::Singleton;

		mmgr_ = mmgr;
		pages_.MMgr(mmgr_);
		return true;
	}

	void MPageMgr::Final()
	{
		RWLocker::ScopedLock locker(guard_);

		//
	}

	bool MPageMgr::Insert(MPageInfo * info)
	{
		if(info == NULL || info->Empty())
			return false;
		RWLocker::ScopedLock locker(guard_);

		return pages_.Insert(info->Addr(), (uintptr_t)info);	
	}
	MPageInfo * MPageMgr::Find(const void * addr) const
	{
		if (addr == NULL)
			return NULL;

		RWLocker::ScopedSharedLock locker(guard_);

		const AddrNode * node = pages_.LowerBound(addr);
		if (node == NULL || node->Data == NULL)
			return NULL;

		MPageInfo * info = (MPageInfo *)node->Data;	
		if(info->IsInside(addr , 0) == true)
			return info;
		else
			return NULL;
	}

	bool MPageMgr::Delete(const void * addr, MPageInfo*& info)
	{
		if (addr == NULL)
			return true;
		RWLocker::ScopedLock locker(guard_);

		const AddrNode * node = pages_.LowerBound(addr);
		if (node == NULL || node->Data == 0)
			return NULL;

		const MPageInfo * data = (const MPageInfo *)node->Data;
		if (data->IsInside(addr, 0) == false)
			return false;
		uintptr_t value = 0;
		if (pages_.Delete(data->Addr(), value) == false)
			return false;

		info = (MPageInfo *)value;
		return true;
	}
	bool MPageMgr::Validate(const void * addr, size_t size) const
	{
		if (addr == NULL)
			return false;

		const void * ptr = addr;
		size_t max_size = size;
		const MPageInfo * info = NULL;

		while ((info = Find(ptr)) != NULL)
		{
			size_t move_size = ((uintptr_t)info->Addr() + info->Size()) - (uintptr_t)ptr;
			if (move_size >= max_size)
				return true;

			max_size -= move_size;
			ptr = (void *)((uintptr_t)ptr + move_size);
		}

		return false;
	}

	bool MPageMgr::Commit(void * addr, size_t size)
	{
		if (addr == NULL)
			return false;
		//确保是kMPageSize的倍数
		if (((uintptr_t)addr & (MPageInfo::kMPageSize - 1)) != 0 || (size & (MPageInfo::kMPageSize - 1)) != 0)
			return false;

		RWLocker::ScopedLock locker(guard_);

		size_t left = size;
		void * ptr = addr;
		while (left > 0)
		{
			uintptr_t value = 0;
			if (pages_.Find(ptr, value) == false || value == 0)
				return false;

			MPageInfo * info = (MPageInfo *)value;
			size_t isize = info->Size();
			if (isize > left)
				isize = left;
			if (info->Commit(ptr, isize) == false)
				return false;

			ptr = (void *)((uintptr_t)ptr + isize);
			left -= isize;
		}

		return true;
	}
	bool MPageMgr::Decommit(void * addr, size_t size)
	{
		if (addr == NULL)
			return false;
		//确保是kMPageSize的倍数
		if (((uintptr_t)addr & (MPageInfo::kMPageSize - 1)) != 0 || (size & (MPageInfo::kMPageSize - 1)) != 0)
			return false;

		RWLocker::ScopedLock locker(guard_);

		size_t left = size;
		void * ptr = addr;
		while (left > 0)
		{
			uintptr_t value = 0;
			if (pages_.Find(ptr, value) == false || value == 0)
				return false;
			MPageInfo * info = (MPageInfo *)value;
			size_t isize = info->Size();
			if (isize > left)
				isize = left;
			if (info->Decommit(ptr, isize) == false)
				return false;

			ptr = (void *)((uintptr_t)ptr + isize);
			left -= isize;
		}

		return true;
	}

	size_t MPageMgr::Size() const
	{
		RWLocker::ScopedSharedLock locker(guard_);
		return pages_.Size();
	}

	static MPageMgr __mpage_mgr_instance__;
	MPageMgr& MPageMgr::Singleton()
	{
		return __mpage_mgr_instance__;
	}


	MMapInfo::MMapInfo()
	{
		Type(MPageInfo::kMMapTypeMem);
	}

	MMapInfo::~MMapInfo()
	{
		Final();
	}

	bool MMapInfo::Init(void * addr, size_t size, int prot, int flags)
	{
		DWORD wflags = MPageInfo::FlagToWin(prot, flags);
		DWORD wprot = MPageInfo::ProtToWin(prot);
		if ((flags & MAP_FIXED) == 0)
			addr = NULL;

		void *map_addr = ::VirtualAlloc(addr, size, wflags, PAGE_READWRITE);
		if (map_addr == NULL)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		Addr(map_addr);
		Size(size);
		Prot(prot);
		Flags(flags);
		WFlags(wflags);		
		return true;
	}

	void MMapInfo::Final()
	{
		RWLocker::ScopedLock locker(guard_);
		void * addr = Addr();
		size_t size = Size();
		if (addr != NULL)
		{
			::VirtualFree(addr, size, MEM_RELEASE);
			Addr(NULL);
		}
		Size(0);	
	}

	bool MMapInfo::CommitAllPages()
	{
		//已经被全部提交过了。
		DWORD wflags = WFlags();
		if (wflags & MEM_COMMIT)
			return true;

		if (::VirtualAlloc(Addr(), Size() , MEM_COMMIT, PAGE_READWRITE) == NULL)
			return false;

		wflags |= MEM_COMMIT;
		WFlags(wflags);
		return true;
	}

	bool MMapInfo::CommitPiece(void * addr, size_t size)
	{
		if (Search(addr, size).Empty() == false)
			return true;

		//区域内分片提交
		if (size == 0)
		{
			//从当前位置，一直到区域结束
			uintptr_t offset = (uintptr_t)addr - (uintptr_t)Addr();
			size = Size() - (size_t)offset;
		}

		if (::VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
		{
			DWORD errcode = ::GetLastError();
			errno = ::oserr_map(errcode);
			return false;
		}

		commits_.Insert(addr, size);
		return true;
	}
	bool MMapInfo::DecommitAllPages()
	{
		::VirtualFree(Addr(), Size(), MEM_DECOMMIT);
		DWORD wflags = WFlags();
		WFlags(wflags &(~MEM_COMMIT));

		if (commits_.Empty() == false)
			commits_.Clear();

		return true;
	}
	bool MMapInfo::DecommitSinglePiece(MPageCommit& commit, void * addr, size_t size)
	{
		//1、先排除单片全部decommit的情况
		if (commit.Addr() == addr)
		{
			size_t desize = size;
			if (desize == 0)
				desize = commit.Size();

			bool result = (::VirtualFree(addr, desize, MEM_DECOMMIT) == TRUE);

			if (desize == commit.Size())
			{
				uintptr_t data_size = 0;
				commits_.Delete(commit.Addr(), data_size);
			}

			return result;
		}

		if ((size == 0) || ((uintptr_t)commit.Addr() + commit.Size()) == (uintptr_t)addr + size)
		{
			//尾部释放
			size_t desize = size;
			if (desize == 0)
				desize = ((uintptr_t)commit.Addr() + commit.Size()) - (uintptr_t)addr;

			bool result = false;
			if (::VirtualFree(addr, desize, MEM_DECOMMIT) == TRUE)
			{
				commit.Size(commit.Size() - desize);
				result = true;
			}
			return result;
		}

		//中段释放，需要分裂出一个新的commit记录
		bool result = false;
		if (::VirtualFree(addr, size, MEM_DECOMMIT) == TRUE)
		{
			void * new_addr = (void *)((uintptr_t)addr + size);
			uintptr_t new_size = ((uintptr_t)commit.Addr() + commit.Size()) - ((uintptr_t)addr + size);
			commits_.Insert(new_addr, new_size);

			uintptr_t left_size = (uintptr_t)addr - (uintptr_t)commit.Addr();
			commit.Size(left_size);
			result = true;
		}

		return result;
	}
	bool MMapInfo::DecommitMultiPiece(MPageCommit& commit, void * addr, size_t size)
	{
		//多次commit，合并decommit
		size_t max_size = size;
		if (size == 0)
			max_size = ((uintptr_t)Addr() + Size()) - (uintptr_t)addr;

		void * ptr = addr;
		bool result = true;
		while (commit.Empty() == false && max_size > 0)
		{
			size_t desize = commit.Size();
			if (desize > max_size)
			{
				desize = max_size;
				commit.Addr((void *)((uintptr_t)ptr + desize));
				commit.Size(commit.Size() - desize);
			}
			else
			{
				uintptr_t data = 0;
				commits_.Delete(ptr, data);
			}

			if (::VirtualFree(ptr, desize, MEM_DECOMMIT) == FALSE)
			{
				result = false;
				break;
			}

			max_size -= desize;
			ptr = (void *)((uintptr_t)ptr + desize);

			commit = Search(ptr, 0);
		}

		return true;
	}


	FMapInfo::FMapInfo()
	{
		Type(MPageInfo::kMMapTypeFile);
		hfile_ = INVALID_HANDLE_VALUE;
		hmap_ = INVALID_HANDLE_VALUE;
	}
	FMapInfo::~FMapInfo()
	{
		Final();
	}

	bool FMapInfo::Init(void * addr, size_t size, int prot, int flags, int fd, off_t offset)
	{
		if (fd == -1)
			return false;
		Object * obj = ObjMgr::Singleton().Get(fd);
		if(obj == NULL)
			return false;
		HANDLE hfile = NULL;		//等后续获取
		DWORD wprot = MPageInfo::ProtToWin(prot);
		DWORD wflags = MPageInfo::FlagToWin(prot, flags);

		wprot |= wflags;
		if (wprot == 0)
			wprot = PAGE_READWRITE | SEC_COMMIT;

		//2、映射内存
		HANDLE hmap = ::CreateFileMappingA(hfile, NULL, wprot, 0, (DWORD)size, NULL);
		if (hmap == NULL)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		void * map_addr = ::MapViewOfFile(hmap, FILE_MAP_ALL_ACCESS, 0, 0, size);
		if (map_addr == NULL)
		{
			DWORD errcode = ::GetLastError();
			::CloseHandle(hmap);
			return NULL;
		}

		FD(fd);
		Prot(prot);
		Flags(flags);
		WFlags(wflags);
		Offset(offset);

		hfile_ = hfile;
		hmap_ = hmap;

		Addr(map_addr);
		Size(size);
		return true;
	}

	void FMapInfo::Final()
	{
		if (Addr() != NULL)
		{
			::UnmapViewOfFile(Addr());
			Addr(NULL);
		}

		if (hmap_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hmap_);
			hmap_ = INVALID_HANDLE_VALUE;
		}
		if (hfile_ != INVALID_HANDLE_VALUE)
		{
			hfile_ = INVALID_HANDLE_VALUE;
		}
	}

	ShmMapInfo::ShmMapInfo()
	{
		Type(MPageInfo::kMMapTypeShm);
	}
	ShmMapInfo::~ShmMapInfo()
	{
		Final();
	}
	bool ShmMapInfo::Init(void * addr, size_t size, int prot, int flags, int fd, off_t offset)
	{
		return false;
	}
	void ShmMapInfo::Final()
	{
		//
	}
}
