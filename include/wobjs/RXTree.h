
#ifndef __QUARK_WOBJS_RXTREE_H
#define __QUARK_WOBJS_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		�����������ڽڵ�Ƚϼ��еĳ��������Ҷ�����Ҫ��Ƚϸߡ������ʵ���У�Ҫ��ÿһ�����볤����һ�µġ�
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
		int per_bits_;	//ÿһ��ı���λ��
		int level_;

		uintptr_t * root_;
	};
}

#endif  /** __QUARK_WOBJS_RXTREE_H */
