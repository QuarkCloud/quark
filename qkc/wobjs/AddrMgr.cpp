
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

		AddrNode *node = (AddrNode *)MemAlloc(sizeof(AddrNode), true);
		node->Assign(addr, data);

		if (Insert(node) == true)
			return true;

		MemFree(node);
		return false;
	}

	bool AddrMgr::Delete(void * addr, void *& data)
	{
		AddrNode tmp(addr, NULL);
		
	}

	bool AddrMgr::Find(void * addr, void *& data) const
	{
	
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

}

