
#include <errno.h>
#include "wobjs/INotifyMgr.h"
#include "wobjs/ErrorMap.h"
#include "wobjs/ObjMgr.h"

namespace qkc {

	INotifyOvlp::INotifyOvlp()
	{
		inited_ = false;
		Init();
	}

	INotifyOvlp::~INotifyOvlp()
	{
		Final();
	}

	bool INotifyOvlp::Init() 
	{
		if (inited_ == true)
			return true;

		::memset(&ovlp_, 0, sizeof(ovlp_));
		owner_ = NULL;
		buffer_ = cache_;
		bufsize_ = 0;

		inited_ = true;
		return inited_;
	}

	void INotifyOvlp::Final()
	{
		if (inited_ == false)
			return;

		//
		inited_ = false;	
	}

	void INotifyOvlp::Owner(INotifyItem * owner)
	{
		if (owner_ != NULL)
			owner_->Ovlp(NULL);

		if (owner != NULL)
			owner->Ovlp(this);
		owner_ = owner;
	}


	INotifyItem::INotifyItem()
	{
		handle_ = NULL;
		occur_ = 0;
		owner_ = NULL;
		ovlp_ = NULL;
		data_ = NULL;
	}

	INotifyItem::~INotifyItem()
	{
		Final();
	}

	bool INotifyItem::Init(const char * name, int mask)
	{
		if (name == NULL)
			return false;

		if (OpenNotifyFile(name) == false)
			return false;

		size_t name_size = ::strlen(name) ;
		size_t data_size = sizeof(struct inotify_event) + name_size + 1 - 4;
		struct inotify_event * data = (struct inotify_event *)::malloc(data_size);
		if (data == NULL)
			return false;

		::memset(data, 0, data_size);
		::memcpy(data->name, name, name_size);
		data->name[name_size] = '\0';

		data->len = name_size + 1;
		data->mask = mask;	
		data_ = data;
		return true;
	}

	void INotifyItem::Final()
	{
		if (data_ != NULL)
		{
			::free(data_);
			data_ = NULL;
		}	
	}

	bool INotifyItem::OpenNotifyFile(const char * name)
	{
		HANDLE handle = ::CreateFileA(name, GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
		if (handle == INVALID_HANDLE_VALUE || handle == NULL)
		{
			DWORD errcode = ::GetLastError();
			errno = oserr_map(errcode);
			return false;
		}

		handle_ = handle;
		return true;
	}

	INotifyItems::INotifyItems()
	{
		//
	}

	INotifyItems::~INotifyItems()
	{
		//
	}

	bool INotifyItems::Insert(INotifyItem * item)
	{
		return RBTree::Insert(item);
	}

	bool INotifyItems::Find(int wd, INotifyItem *& item)
	{
		INotifyItem * cur = (INotifyItem *)Root();

		while (cur != NULL)
		{
			int delta = wd - cur->Key();
			if (delta == 0)
			{
				item = cur;
				return true;
			}
			else if (delta < 0)
				cur = (INotifyItem *)cur->Left;
			else
				cur = (INotifyItem *)cur->Right;
		}
		return false;
	}

	bool INotifyItems::Delete(int wd, INotifyItem *& item)
	{
		item = NULL;
		if (Find(wd, item) == false)
			return false;

		Erase(item);
		return true;
	}

	int INotifyItems::Compare(const RBNode * src, const RBNode * dst) const
	{
		return CompareNode((const INotifyItem *)src, (const INotifyItem *)dst);
	}

	int INotifyItems::CompareNode(const INotifyItem * src, const INotifyItem * dst) const
	{
		if (src == NULL)
			return (dst == NULL) ? 0 : -1;
		else  if (dst == NULL)
			return 1;

		return (src->Key() - dst->Key());
	}

	INotifyMgr::INotifyMgr()
	{
		iocp_ = INVALID_HANDLE_VALUE;
		handle_ = -1;
		count_ = 0;
		last_id_ = 0;
		finaled_ = false;
	}

	INotifyMgr::~INotifyMgr()
	{
		Final();
	}

	void INotifyMgr::SetInfo()
	{
		OType(Object::kFileWatcher);
	}

	int INotifyMgr::Init()
	{
		Mutex::ScopedLock locker(guard_);

		if (iocp_ == INVALID_HANDLE_VALUE || iocp_ == NULL)
		{
			iocp_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if (iocp_ == INVALID_HANDLE_VALUE)
				return -1;

			handle_ = ObjMgr::Singleton().Add(iocp_, this);
			if (handle_ == -1)
				return -1;
		}

		return handle_;	
	}

	void INotifyMgr::Final()
	{
		Mutex::ScopedLock locker(guard_);
		if (finaled_ == true)
			return;

		Object * obj = NULL;
		ObjMgr::Singleton().Delete(handle_ , obj);

		if (iocp_ != INVALID_HANDLE_VALUE && iocp_ != NULL)
		{
			::CloseHandle(iocp_);
			iocp_ = INVALID_HANDLE_VALUE;
		}

		count_ = last_id_ = 0;
		items_.Clear();
		finaled_ = true;
		
	}

	int INotifyMgr::Add(const char * name, uint32_t mask)
	{
		INotifyItem * item = new INotifyItem();
		if (item->Init(name, mask) == false)
		{
			delete item;
			return -1;
		}

		Mutex::ScopedLock locker(guard_);
		if (items_.Insert(item) == false)
			return -1;

		item->Key(++last_id_);
		++count_;
		return last_id_;
	}

	bool INotifyMgr::Del(int wd)
	{
		Mutex::ScopedLock locker(guard_);

		INotifyItem * item = NULL;
		if (items_.Delete(wd, item) == false)
			return false;
		--count_;
		if (item != NULL)
		{
			item->Final();
			delete item;
		}

		return true;
	}

	INotifyItem * INotifyMgr::Find(int wd)
	{
		Mutex::ScopedLock locker(guard_);

		INotifyItem * item = NULL;
		if (items_.Find(wd, item) == false)
			return NULL;

		return item;	
	}
}
