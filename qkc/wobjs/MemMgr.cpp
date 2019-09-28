

#include "wobjs/MemMgr.h"
#include "wobjs/GlobalLocker.h"

namespace qkc {


	MemMgr::MemMgr()
	{
		//
	}

	MemMgr::~MemMgr()
	{
		//
	}

	void * MemMgr::Alloc(size_t size, bool zeroed)
	{
		DWORD flags = 0;
		if (zeroed == true)
			flags = 0x00000008;  //HEAP_ZERO_MEMORY;
		return ::HeapAlloc(::GetProcessHeap(), flags, size);
	}

	void MemMgr::Free(void * addr)
	{
		if(addr != NULL)
			::HeapFree(::GetProcessHeap(), 0, addr);
	}

	MemMgr MemMgr::Singleton;
	static MemMgr * __default_mem_mgr__ = &MemMgr::Singleton;

	MemMgr * MemMgr::Default()
	{
		return __default_mem_mgr__;
	}

	void MemMgr::Default(MemMgr * mgr)
	{
		GlobalLocker::kLocker.Lock();
		if (mgr == NULL)
			__default_mem_mgr__ = &MemMgr::Singleton;
		else
			__default_mem_mgr__ = mgr;
		GlobalLocker::kLocker.Unlock();	
	}

	void * MemAlloc(size_t size, bool zeroed)
	{
		return __default_mem_mgr__->Alloc(size , zeroed);
	}

	void MemFree(void * addr)
	{
		__default_mem_mgr__->Free(addr);
	}
}


