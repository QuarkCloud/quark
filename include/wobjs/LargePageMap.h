
#ifndef __QUARK_WOBJS_LARGE_PAGE_MAP_H
#define __QUARK_WOBJS_LARGE_PAGE_MAP_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"
#include "wobjs/RXTree.h"
#include "wobjs/RList.h"
#include "wobjs/SpinLocker.h"
#include "bits/wordsize.h"
namespace qkc {

	/**
	2020-01-16
	主要解决内存页的地址管理问题	

	2020-03-16
	细则来自于google的tcmalloc
	x86-64和arm64使用的是48位空间，因此我们可以使用2层基数树。32位系统扣除4K的页，也只需要2层

	2020-04-14
	将名称从PageMap改为LargePageMap，主要是因为系统默认的内存页大小为4K。这里是为了参考tcmalloc
	和jemalloc，进行大内存向系统申请，内存进行小内存分配操作用。为了避免引起误解，而专门做出变更。
	*/
	class QKCAPI LargePageInfo : public RLNode {
	public:
		LargePageInfo();
		virtual ~LargePageInfo();

		void Init(void * addr, size_t size);
		void Reset();

		inline void Addr(void * addr) { addr_ = addr; }
		inline void * Addr() { return addr_; }
		inline void Size(size_t size) { size_ = size; }
		inline size_t Size() { return size_; }

		static bool AllocLargePage(MemMgr * mgr , LargePageInfo * page , size_t size);
		static bool FreeLargePage(MemMgr * mgr, LargePageInfo * page);

#if __WORDSIZE == 32

		static const int kPageShift = 12;
		static const int kPageSize = (1 << kPageShift);
		static const int kMaxBits = __WORDSIZE;
		static const int kLevel = 2;
#else

		static const int kPageShift = 16;
		static const int kPageSize = (1 << kPageShift);
		static const int kMaxBits = 48;
		static const int kLevel = 2;
#endif

	private:
		void * addr_;
		size_t size_;
	};

	/**
		PageMap的每个地址只能映射一个页，只提供精确查找的功能。如果某个地址不是页的起始地址，
		而只是页内地址，则无法找到该页。
		2020-04-12
		去掉读写锁，留待真实用到该管理器来实现锁操作
	*/
	class QKCAPI LargePageMap{
	public:
		LargePageMap(MemMgr * mmgr = NULL);
		~LargePageMap();

		bool Init(MemMgr * mmgr = NULL);
		void Final();

		bool Insert(LargePageInfo * page);
		const LargePageInfo * Find(void * addr) const;
		bool Delete(void * addr, LargePageInfo *& page);

		static inline uintptr_t CalcKeyFromAddr(void * addr){
			uintptr_t mask = -1;
			int shift_size = __WORDSIZE - LargePageInfo::kMaxBits;
			if (shift_size != 0)
				mask = (mask << shift_size) >> shift_size;				
			return ((uintptr_t)addr & mask) >> LargePageInfo::kPageShift;
		}

	private:
		RadixTree pages_;
		RList list_;
		MemMgr * mmgr_;
	};
}

#endif  /** __QUARK_WOBJS_LARGE_PAGE_MAP_H */
