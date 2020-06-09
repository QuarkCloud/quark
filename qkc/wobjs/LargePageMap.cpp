
#include "wobjs/LargePageMap.h"

namespace qkc {


	LargePageInfo::LargePageInfo()
	{
		addr_ = NULL;
		size_ = 0;
	}

	LargePageInfo::~LargePageInfo()
	{
		//
	}

	void LargePageInfo::Init(void * addr, size_t size)
	{
		addr_ = addr;
		size_ = size;
	}

	void LargePageInfo::Reset()
	{
		addr_ = NULL;
		size_ = 0;	
	}

	bool LargePageInfo::AllocLargePage(MemMgr * mgr, LargePageInfo * page, size_t size)
	{
		if (page == NULL)
			return false;

		//字节对齐，确保是kPageSize的整数倍
		size_t tmpsize = ((size + kPageSize - 1) / kPageSize) * kPageSize;

		void * addr = NULL;
		if (mgr == NULL)
			addr = MemAlloc(tmpsize);
		else
			addr = mgr->Alloc(tmpsize);

		if (addr == NULL)
			return false;

		page->Init(addr, tmpsize);
		return true;
	}

	bool LargePageInfo::FreeLargePage(MemMgr * mgr, LargePageInfo * page)
	{
		if (page == NULL)
			return true;

		if (mgr == NULL)
			MemFree(page->Addr());
		else
			mgr->Free(page->Addr());

		page->Reset();
		return true;	
	}

	LargePageMap::LargePageMap(MemMgr * mmgr)
	{
		mmgr_ = NULL;
		Init(mmgr);
	}

	LargePageMap::~LargePageMap()
	{
		Final();
	}

	bool LargePageMap::Init(MemMgr * mmgr)
	{
		if (pages_.Init(LargePageInfo::kMaxBits, LargePageInfo::kLevel, mmgr) == false)
			return false;

		mmgr_ = mmgr;
		return true;	
	}

	void LargePageMap::Final()
	{
		while (list_.Empty() == false)
		{
			RLNode * node = list_.Next;
			LargePageInfo * page = (LargePageInfo *)node;
			list_.Del(node);

			uintptr_t value = 0;
			pages_.Delete((uintptr_t)page->Addr(), value);

			LargePageInfo::FreeLargePage(mmgr_, page);
			page->Reset();
		}

		pages_.Final();
	}

	bool LargePageMap::Insert(LargePageInfo * page)
	{
		if (page == NULL)
			return false;

		uintptr_t key = CalcKeyFromAddr(page->Addr());
		if (pages_.Insert(key, (uintptr_t)page) == false)
			return false;
		list_.Add(page);
		return true;
	}

	const LargePageInfo * LargePageMap::Find(void * addr) const
	{
		if (addr == NULL)
			return NULL;
		
		uintptr_t value = 0 ;
		uintptr_t key = CalcKeyFromAddr(addr);;
		if (pages_.Find(key, value) == false || value == 0)
			return NULL;
		return (LargePageInfo *)value;
	}

	bool LargePageMap::Delete(void * addr, LargePageInfo *& page)
	{
		if (addr == NULL)
			return NULL;

		uintptr_t value = 0;
		uintptr_t key = CalcKeyFromAddr(addr);

		if (pages_.Delete(key, value) == false || value == 0)
			return false;

		page = (LargePageInfo *)value;
		list_.Del(page);
		return true;
	}
}
