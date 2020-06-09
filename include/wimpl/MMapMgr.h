
#ifndef __QUARK_WIMP_MMAP_MGR_H
#define __QUARK_WIMP_MMAP_MGR_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "wobjs/MemMgr.h"
#include "wobjs/RList.h"
#include "wobjs/RWLocker.h"
#include "wobjs/AddrMgr.h"

namespace qkc {

	/**
		2020-04-15
		内存映射存在三种情况：	1、是常规的文件映射；2、无文件句柄的纯内存管理；3、以文件句柄形势映射的共享内存。包括shm_open和shmget
		与LargePageMap不同的是，这里页的大小是系统默认的4K，而不是大容量的。
		这里的实现，主要是针对mmap/munmap
	*/
	class QKCAPI MRange {
	public:
		MRange();
		MRange(void * addr , size_t size);
		MRange(const MRange& range);
		virtual ~MRange();
		MRange& operator=(const MRange& range);

		inline const void * Addr() const { return addr_; }
		inline void * Addr() { return addr_; }
		inline void Addr(void * addr) { addr_ = addr; }
		inline size_t Size() const { return size_; }
		inline void Size(size_t size) { size_ = size; }
		inline bool Empty() const { return (addr_ == NULL || size_ == 0); }

		bool IsInside(const void * addr, size_t size) const;
		void Assign(void * addr, size_t size);
		void Assign(const MRange& range);
	private:
		void * addr_;
		size_t size_;
	};

	class QKCAPI MPageCommit {
	public:
		MPageCommit();
		MPageCommit(const MPageCommit& commit);
		MPageCommit(const AddrNode * node);
		virtual ~MPageCommit();

		MPageCommit& operator=(const MPageCommit& commit);
		MPageCommit& operator=(const AddrNode * node);

		const void * Addr() const;
		void * Addr();
		void Addr(const void * addr);
		size_t Size() const;
		void Size(size_t size);
		bool IsInside(const void * addr, size_t size) const;
		inline bool Empty() const { return node_ == NULL; }
	private:
		mutable AddrNode * node_;
	};

	class QKCAPI MPageInfo : public MRange {
	public:
		MPageInfo();
		virtual ~MPageInfo();

		static const int kMMapTypeNone = 0;
		static const int kMMapTypeFile = 1;
		static const int kMMapTypeMem  = 2;
		static const int kMMapTypeShm  = 3;

		inline int FD() const { return fd_; }
		inline void FD(int fd) { fd_ = fd; }
		inline int Prot() const { return prot_; }
		inline void Prot(int prot) { prot_ = prot; }
		inline int Flags() const { return flags_; }
		inline void Flags(int flags) { flags_ = flags; }
		inline DWORD WFlags() const { return wflags_; }
		inline void WFlags(DWORD wflags) {wflags_ = wflags; }

		inline int Type() const { return type_; }
		inline void Type(int type) { type_ = type; }
		inline off_t Offset() const { return offset_; }
		inline void Offset(off_t offset) { offset_ = offset; }

		MPageCommit Find(const void * addr, size_t size) const;

		bool Commit(void * addr, size_t size);
		bool Decommit(void * addr, size_t size);
		virtual void Final();

		static DWORD ProtToWin(int prot);
		static DWORD FlagToWin(int prot , int flag);
		static const char * StrWinFlag(DWORD wflags);

		static const int kMPageSize = 4096;
	protected:
		mutable RWLocker guard_;
		AddrMgr commits_;
		virtual bool CommitAllPages();
		virtual bool CommitPiece(void * addr, size_t size);
		MPageCommit Search(const void * addr, size_t size) const;
		virtual bool DecommitAllPages();
		virtual bool DecommitSinglePiece(MPageCommit& commit, void * addr, size_t size);
		virtual bool DecommitMultiPiece(MPageCommit& commit, void * addr, size_t size);
	private:
		int fd_;
		int prot_;
		int flags_;
		off_t offset_;
		DWORD wflags_;
		int type_;
	};

	//基于MPageInfo和mmap+munmap的输入参数进行管理。而具体的MPageInfo实现由子类完成。
	class QKCAPI MPageMgr {
	public:
		MPageMgr(MemMgr * mmgr = NULL);
		virtual ~MPageMgr();

		bool Init(MemMgr * mmgr = NULL);
		void Final();

		//info插入到RadixTree时，被分裂成多个内存块地址，每个内存块大小为kPageSize。
		bool Insert(MPageInfo * info);
		//查找地址在哪个MMapInfo中，不一定要等于MapAddr。
		MPageInfo * Find(const void * addr) const;
		//删除包含该地址的内存区域，以及该内存区域所切割的所有内存块地址
		bool Delete(const void * addr, MPageInfo*& info);
		bool Validate(const void * addr, size_t size) const;

		bool Commit(void * addr, size_t size);
		bool Decommit(void * addr, size_t size);

		size_t Size() const;

		static MPageMgr& Singleton();

	private:
		//之所以不使用基数树，是因为内存地址可能不是起始地址，而是内存页内部地址，需要搜索该地址所在的内存页
		AddrMgr pages_;
		mutable RWLocker guard_;
		MemMgr * mmgr_;
	};

	class QKCAPI MMapInfo : public MPageInfo {
	public:
		MMapInfo();
		virtual ~MMapInfo();

		bool Init(void * addr, size_t size, int prot, int flags);
		void Final();
	protected:
		bool CommitAllPages();
		bool CommitPiece(void * addr , size_t size);
		bool DecommitAllPages();
		bool DecommitSinglePiece(MPageCommit& commit , void * addr , size_t size);
		bool DecommitMultiPiece(MPageCommit& commit, void * addr, size_t size);
	};


	class QKCAPI FMapInfo : public MPageInfo {
	public:
		FMapInfo();
		virtual ~FMapInfo();
		bool Init(void * addr, size_t size, int prot, int flags , int fd , off_t offset);
		void Final();
	private:
		HANDLE hfile_;
		HANDLE hmap_;
	};

	class QKCAPI ShmMapInfo : public MPageInfo {
	public:
		ShmMapInfo();
		virtual ~ShmMapInfo();
		bool Init(void * addr, size_t size, int prot, int flags, int fd, off_t offset);
		void Final();
	};
}

#endif  /** __QUARK_WIMP_MMAP_MGR_H */
