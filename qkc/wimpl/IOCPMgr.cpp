
#include "wimpl/IOCPMgr.h"
#include <errno.h>
#include "wobjs/ErrorMap.h"
namespace qkc {

	IOCPItem::IOCPItem()
	{
		//
	}

	IOCPItem::~IOCPItem()
	{
		//
	}

	IOCPMgr::IOCPMgr()
	{
		//
	}

	IOCPMgr::~IOCPMgr()
	{
		//
	}

	bool IOCPMgr::Init(DWORD backlog)
	{
		HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, backlog);
		if (handle == NULL)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		iocp_ = handle;
		return true;
	}

	void IOCPMgr::Final()
	{
		//
	}

	bool IOCPMgr::Add(IOCPItem * item, uint32_t events)
	{
		if (item == NULL || events == 0)
			return false;

		RWLocker::ScopedLock locker(guard_);

		HANDLE handle = ::CreateIoCompletionPort(item->Handle(), iocp_, (ULONG_PTR)item, 0);
		if(handle != iocp_)
			return false;

		return items_.Insert(item, (uintptr_t)item);
	}

	bool IOCPMgr::Del(IOCPItem * item)
	{
		if(item == NULL)
			return false;

		RWLocker::ScopedLock locker(guard_);
		uintptr_t value = 0;
		if (items_.Delete(item, value) == false || value == 0)
			return false;

		return ((uintptr_t)item == value);
	}

	bool IOCPMgr::Modify(IOCPItem * item, uint32_t events)
	{
		if(item == NULL)
			return false;

		RWLocker::ScopedLock locker(guard_);
		uintptr_t value = 0;
		if (items_.Find(item, value) == false)
			return false;

		return true;
	}

	int IOCPMgr::Wait(IOCPItem **items, int max_size, int timeout)
	{
		if (items == NULL || max_size <= 0)
			return -1;
		/**
			2020-04-28
			等后续网络时继续完善
		*/

		DWORD bytes = 0; 
		ULONG_PTR key = 0;
		OVERLAPPED * ovlp = NULL;

		thread_count_.Increment();
		BOOL result = ::GetQueuedCompletionStatus(iocp_, &bytes, &key, &ovlp, timeout);
		LONG threads_count = thread_count_.Decrement();
		if(result == FALSE)
		{
			DWORD errcode = ::GetLastError();
			if (errcode == ERROR_TIMEOUT)
				result = TRUE;
		}
		else
		{
			if (ovlp == NULL && key == 1)
			{
				if (thread_count_.Value() > 0)
					::PostQueuedCompletionStatus(iocp_, 0, key, NULL);
				return 0;
			}
		}

		return (result==TRUE?0:1);
	}
}

