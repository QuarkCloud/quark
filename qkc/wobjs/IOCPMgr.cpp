
#include "wobjs/IOCPMgr.h"
#include "wobjs/ObjMgr.h"
#include <errno.h>

namespace qkc {

	IOCPOvlp::IOCPOvlp()
	{
		::memset(this, 0, sizeof(*this));
	}

	IOCPOvlp::~IOCPOvlp()
	{
		Final();
	}

	bool IOCPOvlp::Init()
	{
		return true;
	}

	void IOCPOvlp::Final()
	{
		//
	}

	bool IOCPOvlp::Lock()
	{
		return (::InterlockedCompareExchange(&counter_, 1, 0) == 0);
	}

	bool IOCPOvlp::Unlock()
	{
		return (::InterlockedCompareExchange(&counter_, 0, 1) == 1);
	}


	IOCPItem::IOCPItem()
	{
		fd_ = -1;
		type_ = kTypeUnknown;
		::memset(&data_, 0, sizeof(data_));
		mask_ = 0;
		owner_ = NULL;
		callback_ = NULL;
	}

	IOCPItem::~IOCPItem()
	{
		Remove();
	}

	/***
		private:
			int epfd_;
			finaled_
			HANDLE iocp_;
			Mutex	guard_;
			RList readies_;					//已经就绪的
			int ready_counter_;
			volatile LONG thread_counter_;	//加入的线程数
		};
	*/

	IOCPMgr::IOCPMgr()
	{
		epfd_ = -1;
		finaled_ = false;
		iocp_ = INVALID_HANDLE_VALUE;
		ready_counter_ = 0;
		thread_counter_ = 0;
	}

	IOCPMgr::~IOCPMgr()
	{
		Final();
	}

	int IOCPMgr::Init()
	{
		Mutex::ScopedLock locker(guard_);
		HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (handle == INVALID_HANDLE_VALUE)
		{
			errno = EIO;
			return -1;
		}

		int fd = ObjMgr::Singleton().Add(handle, this);
		if (fd < 0)
		{
			CloseHandle(handle);
			return -1;
		}

		iocp_ = handle;
		epfd_ = fd;
		return fd;
	}

	void IOCPMgr::SetInfo()
	{
		OType(Object::kIOCP);
	}

	void IOCPMgr::Final()
	{
		Mutex::ScopedLock locker(guard_);
		if (finaled_ == true)
			return;
		finaled_ = true;

		Object * obj = NULL;
		ObjMgr::Singleton().Delete(iocp_, obj);

		FreeItems();
		if (iocp_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(iocp_);
			iocp_ = INVALID_HANDLE_VALUE;
		}

		epfd_ = -1;
		ready_counter_ = 0;
		thread_counter_ = 0;
	}

	bool IOCPMgr::Add(int fd, struct epoll_event * ev)
	{
		Mutex::ScopedLock locker(guard_);

		Object * obj = ObjMgr::Singleton().Get(fd);
		if (obj == NULL)
			return false;

		/***
			2019-12-12
			管道的问题放到后面来讨论
		*/
		if (obj->IsFile() == false || obj->IsSocket() == false) //|| obj->IsPipe() == false)
			return false;

		if (obj->IsFile())
		{
			//
		}
		else if (obj->IsSocket())
		{

		}

		return false;	
	}

	bool IOCPMgr::Modify(int fd, struct epoll_event * ev)
	{
		return false;
	}

	bool IOCPMgr::Del(int fd, struct epoll_event * ev)
	{
		return false;
	}


	int IOCPMgr::Wait(int timeout)
	{
		DWORD start_time = ::GetTickCount();
		DWORD bytes_transferred = 0;
		ULONG_PTR completion_key = 0;
		LPOVERLAPPED overlapped = NULL;

		::InterlockedIncrement(&thread_counter_);
		BOOL result = ::GetQueuedCompletionStatus(iocp_, &bytes_transferred, &completion_key, &overlapped, timeout);
		LONG now_threads = ::InterlockedDecrement(&thread_counter_);

		if (result == FALSE && overlapped == NULL)
		{
			//没有可等待的事件
			return -1;
		}

		/***
		iocp_ovlp_t * ovlp = (iocp_ovlp_t *)overlapped;
		iocp_item_t * item = (ovlp == NULL) ? NULL : ovlp->owner;

		uint32_t old_occur = 0, new_occur = 0;
		uint32_t events = 0;
		if (item != NULL)
		{
			old_occur = item->occur;
			events = item->data.events;
		}

		if (result == TRUE)
		{
			if (overlapped == NULL && completion_key == 1)
			{
				if (now_threads > 0)
					::PostQueuedCompletionStatus(mgr->iocp, 0, completion_key, NULL);
				return 0;
			}

			ovlp_type_t type = ovlp->type;
			if (type == OVLP_INPUT)
			{
				if (bitop_in(events, EPOLLIN) == true)
				{
					//标记可读
					new_occur |= EPOLLIN;
				}
			}
			else if (type == OVLP_OUTPUT)
			{
				if (bitop_in(events, EPOLLOUT) == true)
				{
					//标记可写
					new_occur |= EPOLLOUT;
				}
			}
		}
		else
		{
			if ((item != NULL) && (bitop_in(events, EPOLLERR) == true))
			{
				new_occur |= EPOLLERR;
			}
		}

		bitop_set(item->occur, new_occur);
		new_occur = item->occur;
		iocp_ovlp_unlock(ovlp);

		bool is_et = bitop_in(events, EPOLLET);

		if (old_occur == new_occur)
		{
			if (new_occur != 0 && is_et == false)
				iocp_mgr_item_ready(mgr, item);
		}
		else
		{
			if (new_occur == 0)
				iocp_mgr_item_unready(mgr, item);
			else if (is_et == false)
				iocp_mgr_item_ready(mgr, item);
		}
		*/

		return 0;
	}

	int IOCPMgr::Retrieve(struct epoll_event * evs, int max_count)
	{
		return 0;
	}

	void IOCPMgr::FreeItems()
	{
		RLNode * cur = NULL;
		while (readies_.Empty() == false)
		{
			cur = readies_.Next;
			cur->Remove();

			IOCPItem * item = (IOCPItem *)cur;
			delete item;
		}
	}

}
