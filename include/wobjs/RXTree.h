
#ifndef __QUARK_WOBJS_RXTREE_H
#define __QUARK_WOBJS_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		基数树，用于节点比较集中的场景，而且对性能要求比较高。在这个实现中，要求每一层掩码长度是一致的。
		分层算法，确保叶子的长度不会大于枝干的长度，特别对于最大比特数不能被分层数整除的时候。
		如： 47 比特位被分为3层，则高两层为16比特位，叶子占剩余的15位。
	*/
	class QKCAPI RadixTree {
	public:
		RadixTree();
		virtual ~RadixTree();

		bool Init(int max_bits, int level, MemMgr * mmgr = NULL);
		void Final();

		bool Insert(uintptr_t key, uintptr_t value);
		bool Find(uintptr_t key, uintptr_t& value) const;
		bool Delete(uintptr_t key, uintptr_t& value);

		static const int kMaxBitCap = 16;		//每层最大比特位容量，不能超过。
		static const int kMaxLevel = 64;		//支持最大层次
	private:
		MemMgr * mmgr_;
		int max_bits_;
		int *bits_;  //动态分配层级
		int level_;

		uintptr_t * root_;
		inline uintptr_t CalcMask(int bits) const {	uintptr_t mask = 1;	return ((mask << bits) - 1);}
		int CalcPath(uintptr_t key, uintptr_t * path, int count) const;
		int CalcPath(uintptr_t * path, int count , uintptr_t * nodes) const;
	};
}

#endif  /** __QUARK_WOBJS_RXTREE_H */
