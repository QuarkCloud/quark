
#include "wobjs/AddrMgr.h"

namespace qkc {

	AddrNode::AddrNode()
	{
		Assign(NULL, NULL);
	}

	AddrNode::AddrNode(void * addr, void * data)
	{
		Assign(addr, data);
	}

	void AddrNode::Assign(void * addr, void * data)
	{
		Addr = addr;
		Data = data;
	}

	AddrMgr::AddrMgr()
	{
		//
	}

	AddrMgr::~AddrMgr()
	{
		//	
	}

	bool AddrMgr::Insert(void * addr, void * data)
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


		AddrNode *node = (AddrNode *)MemAlloc(sizeof(AddrNode), true);
		node->Assign(addr, data);

		node->Link(parent, link);
		InternalInsert(node, false, NULL);
		return true;
	}

	bool AddrMgr::Delete(void * addr , AddrNode *& node)
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

		node = (AddrNode *)cur;

		RBNode * rebalance = InternalErase(node, NULL);
		if(rebalance != NULL)
			InternalEraseColor(rebalance);
		return true;
	}

	bool AddrMgr::Find(void * addr, const AddrNode *& node) const
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

		node = (const AddrNode *)cur;
		return (node != NULL);
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

}

