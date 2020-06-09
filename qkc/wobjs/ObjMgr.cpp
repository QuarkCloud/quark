
#include "wobjs/ObjMgr.h"

namespace qkc {

	ObjMgrNode::ObjMgrNode()
	{
		Clear();
	}

	ObjMgrNode::ObjMgrNode(int idx, void * addr, Object * obj)
	{
		Assign(idx, addr, obj);
	}

	ObjMgrNode::~ObjMgrNode()
	{
		//
	}

	void ObjMgrNode::Clear()
	{
		Index = -1;
		Addr = NULL;
		Obj = NULL;
	}

	void ObjMgrNode::Assign(int idx, void * addr, Object * obj)
	{
		Index = idx;
		Addr = addr;
		Obj = obj;
	}

	ObjMgr::ObjMgr()
	{
		items_ = NULL;
		capacity_ = size_ = last_ = 0;
	}

	ObjMgr::~ObjMgr()
	{
		//
	}

	bool ObjMgr::CheckExtend() const
	{
		return (size_ * 8) > (capacity_ * 7);
	}

	bool ObjMgr::CheckShrink() const
	{
		return (size_ * 8) < capacity_;
	}

	ObjMgr& ObjMgr::Singleton()
	{
		static ObjMgr __obj_mgr_singlton__;
		return __obj_mgr_singlton__;
	}


	int ObjMgr::Add(void * addr, Object * obj)
	{
		guard_.Lock();

		//1、判断空间是否足够
		bool extend = false;
		if (items_ == NULL)
			extend = true;
		else
			extend = CheckExtend();

		//2、空间不足，需要扩展空间
		if (extend == true)
		{
			int capacity = 0;
			if (items_ == NULL || capacity_ == 0)
				capacity = kMinCapacity;
			else
				capacity = capacity_ << 1;
			
			size_t alloc_size = sizeof(ObjMgrNode) * capacity;
			ObjMgrNode * items = (ObjMgrNode*)MemAlloc(alloc_size, true);
			if (items == NULL)
			{
				guard_.Unlock();
				return -1;
			}
			ObjMgrNode* tmp = items_;
			if (items_ != NULL)
			{
				for (int idx = 0; idx < capacity_; ++idx)
					items[idx] = items_[idx];
			}
			items_ = items;
			capacity_ = capacity;

			if (tmp != NULL)
				MemFree(tmp);
		}

		//3、选择一个下标。尽量往后，减少短时间内碰撞的可能性。
		int index = last_ % capacity_ , counter = 0;
		while ((counter < capacity_) && (items_[index].IsEmpty() == false))
		{
			++index;
			if (index >= capacity_)
				index = 0;
			++counter;
		}

		//4、找到合适的节点
		bool success = false;
		if (counter < capacity_)
		{
			if (addrs_.Insert(addr, index) == true)
			{
				items_[index].Assign(index , addr , obj);
				success = true;
				size_++;
				last_ = index + 1;
			}
		}

		if (success == false)
			index = -1;

		obj->index_ = index;

		guard_.Unlock();

		return index;
	}

	bool ObjMgr::Delete(int index, Object*& obj)
	{
		bool result = false;

		guard_.Lock();

		if (index >= 0 && index < capacity_)
		{
			if (items_[index].IsEmpty() == false)
			{
				ObjMgrNode * node = items_ + index;
				obj = node->Obj;
				void * addr = node->Addr;
				items_[index].Clear();
				--size_;
				result = true;

				uintptr_t data = 0;
				addrs_.Delete(addr, data);
			}
		}

		guard_.Unlock();

		return result;
	
	}

	bool ObjMgr::Delete(void * addr, Object *& obj)
	{
		volatile LONG64 value = 0;
		InterlockedIncrement64(&value);
		return false;
	
	}

	const Object * ObjMgr::Get(int idx) const
	{
		const Object * result = NULL;
		guard_.LockShared();
		if (idx >= 0 && idx < size_)
			result = items_[idx].Obj;
		guard_.UnlockShared();

		return result;
	}

	Object * ObjMgr::Get(int idx)
	{
		Object * result = NULL;
		guard_.LockShared();
		if (idx >= 0 && idx < size_)
			result = items_[idx].Obj;
		guard_.UnlockShared();
		return result;
	}

	const Object * ObjMgr::Find(void * addr) const
	{
		const Object * result = NULL;
		uintptr_t data = 0;
		guard_.LockShared();
		if (addrs_.Find(addr, data) == true && data != 0)
		{
			int index = data;
			if (index >= 0 && index < size_)
				result = items_[index].Obj;
		}
		guard_.UnlockShared();		
		return result;
	}

	Object * ObjMgr::Find(void * addr)
	{
		Object * result = NULL;
		uintptr_t data = 0;
		guard_.LockShared();

		if (addrs_.Find(addr, data) == true && data != 0)
		{
			int index = data;
			if (index >= 0 && index < size_)
				result = items_[index].Obj;
		}
		guard_.UnlockShared();
		return result;
	}
}
