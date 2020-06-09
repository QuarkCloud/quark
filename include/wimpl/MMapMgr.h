
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
		�ڴ�ӳ��������������	1���ǳ�����ļ�ӳ�䣻2�����ļ�����Ĵ��ڴ����3�����ļ��������ӳ��Ĺ����ڴ档����shm_open��shmget
		��LargePageMap��ͬ���ǣ�����ҳ�Ĵ�С��ϵͳĬ�ϵ�4K�������Ǵ������ġ�
		�����ʵ�֣���Ҫ�����mmap/munmap
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

	//����MPageInfo��mmap+munmap������������й����������MPageInfoʵ����������ɡ�
	class QKCAPI MPageMgr {
	public:
		MPageMgr(MemMgr * mmgr = NULL);
		virtual ~MPageMgr();

		bool Init(MemMgr * mmgr = NULL);
		void Final();

		//info���뵽RadixTreeʱ�������ѳɶ���ڴ���ַ��ÿ���ڴ���СΪkPageSize��
		bool Insert(MPageInfo * info);
		//���ҵ�ַ���ĸ�MMapInfo�У���һ��Ҫ����MapAddr��
		MPageInfo * Find(const void * addr) const;
		//ɾ�������õ�ַ���ڴ������Լ����ڴ��������и�������ڴ���ַ
		bool Delete(const void * addr, MPageInfo*& info);
		bool Validate(const void * addr, size_t size) const;

		bool Commit(void * addr, size_t size);
		bool Decommit(void * addr, size_t size);

		size_t Size() const;

		static MPageMgr& Singleton();

	private:
		//֮���Բ�ʹ�û�����������Ϊ�ڴ��ַ���ܲ�����ʼ��ַ�������ڴ�ҳ�ڲ���ַ����Ҫ�����õ�ַ���ڵ��ڴ�ҳ
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
