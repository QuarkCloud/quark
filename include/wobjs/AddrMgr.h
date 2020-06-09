
#ifndef __QUARK_WOBJS_ADDR_MGR_H
#define __QUARK_WOBJS_ADDR_MGR_H 1

#include <quark_compile.h>
#include "wobjs/RBTree.h"
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		先用红黑树，性能和内存是可控的。暂时不用基数树。
	*/
	class QKCAPI AddrNode : public RBNode {
	public:
		AddrNode();
		AddrNode(const AddrNode& node);
		explicit AddrNode(void * addr , uintptr_t data);
		AddrNode& operator=(const AddrNode& node);

		void Assign(void * addr, uintptr_t data);
		void Assign(const AddrNode& node);

		void Reset();

		void * Addr;
		uintptr_t Data;
	};

	class QKCAPI AddrMgr : public RBTree{
	public:
		AddrMgr(MemMgr * mmgr = NULL);
		virtual ~AddrMgr();

		typedef AddrNode Node;

		bool Insert(void * addr, uintptr_t data);
		bool Update(void * addr, uintptr_t data);
		bool Delete(const void * addr, uintptr_t& data);
		bool Find(const void * addr, uintptr_t& data) const;
		void Clear();

		// <= addr
		AddrNode *LowerBound(const void * addr) const;
		// >= addr
		AddrNode *UpperBound(const void * addr) const;

		const AddrNode * First() const;
		AddrNode * First();
		const AddrNode * Last() const;
		AddrNode * Last();
		const AddrNode * Prev(const AddrNode * node) const;
		AddrNode * Prev(AddrNode * node);
		const AddrNode * Next(const AddrNode * node) const;
		AddrNode * Next(AddrNode * node);

		inline bool Empty() const { return RBTree::Empty(); }
		inline const AddrNode * Root() const { return (const AddrNode *)RBTree::Root(); }
		inline AddrNode * Root() { return (AddrNode *)RBTree::Root(); }
		inline MemMgr * MMgr() { return mmgr_; }
		inline void MMgr(MemMgr * mmgr) { mmgr_ = mmgr; }
	protected:
		int Compare(const RBNode * src, const RBNode * dst) const;
		int Compare(const void * src, const void * dst) const;
	private:
		AddrNode * NodeAlloc();
		void NodeFree(AddrNode * node);
		MemMgr * mmgr_;
	};
}

#endif  /** __QUARK_WOBJS_ADDR_MGR_H */
