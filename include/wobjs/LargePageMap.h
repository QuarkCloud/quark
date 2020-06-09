
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
	��Ҫ����ڴ�ҳ�ĵ�ַ��������	

	2020-03-16
	ϸ��������google��tcmalloc
	x86-64��arm64ʹ�õ���48λ�ռ䣬������ǿ���ʹ��2���������32λϵͳ�۳�4K��ҳ��Ҳֻ��Ҫ2��

	2020-04-14
	�����ƴ�PageMap��ΪLargePageMap����Ҫ����ΪϵͳĬ�ϵ��ڴ�ҳ��СΪ4K��������Ϊ�˲ο�tcmalloc
	��jemalloc�����д��ڴ���ϵͳ���룬�ڴ����С�ڴ��������á�Ϊ�˱���������⣬��ר�����������
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
		PageMap��ÿ����ַֻ��ӳ��һ��ҳ��ֻ�ṩ��ȷ���ҵĹ��ܡ����ĳ����ַ����ҳ����ʼ��ַ��
		��ֻ��ҳ�ڵ�ַ�����޷��ҵ���ҳ��
		2020-04-12
		ȥ����д����������ʵ�õ��ù�������ʵ��������
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
