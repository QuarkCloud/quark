
#ifndef __QUARK_WOBJS_MEM_MGR_H
#define __QUARK_WOBJS_MEM_MGR_H 1

#include <quark_compile.h>

namespace qkc {

	class QKCAPI MemMgr {
	public:
		MemMgr();
		virtual ~MemMgr();

		virtual void * Alloc(size_t size , bool zeroed = false);
		virtual void Free(void * addr);

		static MemMgr * Default();
		static void Default(MemMgr * mgr);

		static MemMgr Singleton;
	};

	QKCAPI void * MemAlloc(size_t size, bool zeroed = false);
	QKCAPI void MemFree(void * addr);
}

#endif  /** __QUARK_WOBJS_MEM_MGR_H */
