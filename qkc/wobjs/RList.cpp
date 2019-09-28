
#include "wobjs/RList.h"


namespace qkc {

	RLNode::RLNode()
	{
		Init();
	}

	void RLNode::Init()
	{
		Prev = Next = this;
	}

	bool RLNode::Empty() const
	{
		return ((Prev == this) && (Next == this));
	}

	void RList::Add(RLNode * entry)
	{
		RLNode * last = Prev;
		last->Next = entry;
		entry->Prev = last;
		entry->Next = this;
		Prev = entry;
	}

	void RList::Del(RLNode * entry)
	{
		RLNode * prev = entry->Prev;
		RLNode * next = entry->Next;

		prev->Next = next;
		next->Prev = prev;

		entry->Init();
	}


}

