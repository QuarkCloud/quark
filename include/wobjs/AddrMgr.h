
#ifndef __QUARK_WOBJS_ADDR_MGR_H
#define __QUARK_WOBJS_ADDR_MGR_H 1

#include <quark_compile.h>
#include "wobjs/RBTree.h"
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		先用红黑树，性能和内存还是可控的。暂时不用基数树。
	*/
	class QKCAPI AddrNode : public RBNode {
	public:
		AddrNode();
		explicit AddrNode(void * addr , void * data);
		void Assign(void * addr, void * data);
		void * Addr;
		void * Data;
	};

	class QKCAPI AddrMgr : private RBTree{
	public:
		AddrMgr();
		virtual ~AddrMgr();

		bool Insert(void * addr, void * data);
		bool Delete(void * addr, void *& data);
		bool Find(void * addr, void *& data) const;

		int Compare(const RBNode * src, const RBNode * dst) const;
	private:
		//
	};
}

#endif  /** __QUARK_WOBJS_ADDR_MGR_H */
