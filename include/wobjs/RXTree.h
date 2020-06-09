
#ifndef __QUARK_WOBJS_RXTREE_H
#define __QUARK_WOBJS_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		�����������ڽڵ�Ƚϼ��еĳ��������Ҷ�����Ҫ��Ƚϸߡ������ʵ���У�Ҫ��ÿһ�����볤����һ�µġ�
		�ֲ��㷨��ȷ��Ҷ�ӵĳ��Ȳ������֦�ɵĳ��ȣ��ر���������������ܱ��ֲ���������ʱ��
		�磺 47 ����λ����Ϊ3�㣬�������Ϊ16����λ��Ҷ��ռʣ���15λ��
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

		static const int kMaxBitCap = 16;		//ÿ��������λ���������ܳ�����
		static const int kMaxLevel = 64;		//֧�������
	private:
		MemMgr * mmgr_;
		int max_bits_;
		int *bits_;  //��̬����㼶
		int level_;

		uintptr_t * root_;
		inline uintptr_t CalcMask(int bits) const {	uintptr_t mask = 1;	return ((mask << bits) - 1);}
		int CalcPath(uintptr_t key, uintptr_t * path, int count) const;
		int CalcPath(uintptr_t * path, int count , uintptr_t * nodes) const;
	};
}

#endif  /** __QUARK_WOBJS_RXTREE_H */
