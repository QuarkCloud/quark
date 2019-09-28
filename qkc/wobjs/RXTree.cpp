
#include "wobjs/RXTree.h"

namespace qkc {

	RadixTree::RadixTree()
	{
		mmgr_ = NULL;
		max_bits_ = per_bits_ = 0;
		level_ = 0;
		root_ = NULL;
	}

	RadixTree::~RadixTree()
	{
		Final();
	}

	bool RadixTree::Init(int max_bits, int level, MemMgr * mmgr)
	{
		if (level <= 0 || max_bits <= level)
			return false;

		if (mmgr == NULL)
			mmgr_ = MemMgr::Default();
		else
			mmgr_ = mmgr;

		//计算每一层的比特位数，需要注意的是，最后一层不一定是正好的
		per_bits_ = (max_bits + level - 1) / level;	
		level_ = level;
		max_bits_ = max_bits;

		size_t bits = 1;
		size_t rsize = sizeof(uintptr_t) * (bits << per_bits_);
		root_ = (uintptr_t *)mmgr_->Alloc(rsize , true);
		return (root_ != NULL);
	}

	void RadixTree::Final()
	{
		//uintptr_t * stack[256];
		//暂时先放着
	}

	bool RadixTree::Insert(uintptr_t key, uintptr_t value)
	{
		return false;
	}

	bool RadixTree::Find(uintptr_t key, uintptr_t& value)
	{
		return false;
	}

	bool RadixTree::Delete(uintptr_t key, uintptr_t& value)
	{
		return false;
	}

}
