
#ifndef __QUARK_WOBJS_RXTREE_H
#define __QUARK_WOBJS_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		基数树，用于节点比较集中的场景，而且对性能要求比较高。在这个实现中，要求每一层掩码长度是一致的。
	*/
	class QKCAPI RadixTree {
	public:
		RadixTree();
		virtual ~RadixTree();

		bool Init(int max_bits, int level, MemMgr * mmgr = NULL);
		void Final();

		bool Insert(uintptr_t key, uintptr_t value);
		bool Find(uintptr_t key, uintptr_t& value);
		bool Delete(uintptr_t key, uintptr_t& value);

	private:
		MemMgr * mmgr_;
		int max_bits_;
		int per_bits_;	//每一层的比特位数
		int level_;

		uintptr_t * root_;
	};
}

#endif  /** __QUARK_WOBJS_RXTREE_H */
