
#include "wobjs/AddrMgr.h"

namespace qkc {

	AddrNode::AddrNode()
	{
		Assign(NULL, NULL);
	}
	AddrNode::AddrNode(const AddrNode& node)
	{
		Assign(node);
	}

	AddrNode::AddrNode(void * addr, uintptr_t data)
	{
		Assign(addr, data);
	}

	AddrNode& AddrNode::operator=(const AddrNode& node)
	{
		Assign(node);
		return (*this);
	}

	void AddrNode::Assign(void * addr, uintptr_t data)
	{
		Addr = addr;
		Data = data;
	}
	void AddrNode::Assign(const AddrNode& node)
	{
		Addr = node.Addr;
		Data = node.Data;

		RBNode::Assign(node);		
	}

	void AddrNode::Reset()
	{
		Addr = NULL;
		Data = 0;
		RBNode::Reset();
	}

	AddrMgr::AddrMgr(MemMgr * mmgr)
	{
		mmgr_ = mmgr;
		if (mmgr_ == NULL)
			mmgr_ = &MemMgr::Singleton;
	}

	AddrMgr::~AddrMgr()
	{
		//	
	}

	bool AddrMgr::Insert(void * addr, uintptr_t data)
	{
		RBNode * parent = NULL, **link = &root_;
		
		while ((*link) != NULL)
		{
			parent = *link;
			void * key = ((AddrNode *)parent)->Addr;
			intptr_t diff = (intptr_t)key - (intptr_t)addr;
			if (diff == 0)
				return false;
			if (diff < 0)
				*link = parent->Left;
			else
				*link = parent->Right;
		}


		AddrNode *node = NodeAlloc();
		node->Assign(addr, data);

		node->Link(parent, link);
		InternalInsert(node, false, NULL);
		return true;
	}

	bool AddrMgr::Delete(const void * addr , uintptr_t& data)
	{
		RBNode *cur = Root();
		while (cur != NULL)
		{
			void * key = ((AddrNode *)cur)->Addr;
			intptr_t diff = (intptr_t)key - (intptr_t)addr;
			if (diff == 0)
				break;

			if (diff < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		if (cur == NULL)
			return false;

		data = ((AddrNode *)cur)->Data;
		RBNode * rebalance = InternalErase(cur, NULL);
		if(rebalance != NULL)
			InternalEraseColor(rebalance);

		NodeFree((AddrNode *)cur);
		return true;
	}

	bool AddrMgr::Find(const void * addr, uintptr_t& data) const
	{
		const RBNode *cur = Root();
		while (cur != NULL)
		{
			void * key = ((AddrNode *)cur)->Addr;
			intptr_t diff = (intptr_t)key - (intptr_t)addr;
			if (diff == 0)
				break ;

			if (diff < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		if (cur == NULL)
			return false;

		data = ((const AddrNode *)cur)->Data;
		return true;
	}

	bool AddrMgr::Update(void * addr, uintptr_t data)
	{
		RBNode *cur = Root();
		while (cur != NULL)
		{
			void * key = ((AddrNode *)cur)->Addr;
			intptr_t diff = (intptr_t)key - (intptr_t)addr;
			if (diff == 0)
				break;

			if (diff < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		if (cur == NULL)
			return false;
		

		((AddrNode *)cur)->Data = data;
		return true;
	}

	void AddrMgr::Clear()
	{
		AddrNode * cur = First(), *next = NULL;
		while (cur != NULL)
		{
			next = Next(cur);
			NodeFree(cur);	
			cur = next;
		}	
	}

	AddrNode *AddrMgr::LowerBound(const void * addr) const
	{
		if (addr == NULL)
			return NULL;
		const AddrNode * cur = (const AddrNode *)Root() , *result = NULL;
		while (cur != NULL)
		{
			int diff = Compare(cur->Addr, addr);
			if (diff == 0)
			{
				result = cur;
				break;
			}
			else if (diff < 0)
			{
				cur = (const AddrNode *)cur->Right;
			}
			else
			{
				result = cur;
				cur = (const AddrNode *)cur->Left;
			}
		}

		return (AddrNode *)result;
	}

	AddrNode *AddrMgr::UpperBound(const void * addr) const
	{
		if (addr == NULL)
			return NULL;

		const AddrNode * cur = (const AddrNode *)Root(), *parent = NULL, *gparent = NULL;
		intptr_t prev_diff = 0;
		while (cur != NULL)
		{
			intptr_t diff = (intptr_t)addr - (intptr_t)cur->Addr;
			if (diff == 0)
				return (AddrNode *)cur;

			gparent = parent;
			parent = cur;
			if (diff < 0)
			{
				if (prev_diff > 0)

					cur = (const AddrNode *)cur->Left;
			}
			else
				cur = (const AddrNode *)cur->Right;
		}

		//没有找到相等的，那就是到了叶子节点。
		if (parent == NULL)
			return NULL;

		intptr_t diff = (intptr_t)addr - (intptr_t)parent->Addr;
		if (diff < 0)
			return (AddrNode *)parent;

		if (gparent == NULL || parent == gparent->Right)
			return NULL;

		diff = (intptr_t)addr - (intptr_t)gparent->Addr;
		if (diff < 0)
			return (AddrNode *)gparent;
		return NULL;
	}

	int AddrMgr::Compare(const RBNode * src, const RBNode * dst) const
	{
		const AddrNode * n1 = (const AddrNode *)src;
		const AddrNode * n2 = (const AddrNode *)dst;

		intptr_t result = (intptr_t)(n1->Addr) - (intptr_t)(n2->Addr);	
		if (result < 0)
			return -1;
		else if (result > 0)
			return 1;
		return 0;
	}

	int AddrMgr::Compare(const void * src, const void * dst) const
	{
		intptr_t result = (intptr_t)src - (intptr_t)dst;
		if (result < 0)
			return -1;
		else if (result > 0)
			return 1;
		return 0;
	}

	const AddrNode * AddrMgr::First() const
	{
		return (const AddrNode *)RBTree::First();
	}

	AddrNode * AddrMgr::First()
	{
		return (AddrNode *)RBTree::First();
	}

	const AddrNode * AddrMgr::Last() const
	{
		return (const AddrNode *)RBTree::Last();
	}

	AddrNode * AddrMgr::Last()
	{
		return (AddrNode *)RBTree::Last();
	}

	const AddrNode * AddrMgr::Prev(const AddrNode * node) const 
	{
		return (const AddrNode *)RBTree::Prev(node);
	}

	AddrNode * AddrMgr::Prev(AddrNode * node)
	{
		return (AddrNode *)RBTree::Prev(node);
	}

	const AddrNode * AddrMgr::Next(const AddrNode * node) const
	{
		return (const AddrNode *)RBTree::Next(node);
	}

	AddrNode * AddrMgr::Next(AddrNode * node)
	{
		return (AddrNode *)RBTree::Next(node);
	}

	AddrNode * AddrMgr::NodeAlloc()
	{
		size_t size = sizeof(AddrNode);
		return (AddrNode *)mmgr_->Alloc(size, true);
	}

	void AddrMgr::NodeFree(AddrNode * node)
	{
		if (node == NULL)
			return;
		mmgr_->Free(node);
	}
}

