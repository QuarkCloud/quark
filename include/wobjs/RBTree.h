
#ifndef __QUARK_WOBJS_RBTREE_H
#define __QUARK_WOBJS_RBTREE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	/**
		2019-08-07
		本来从Haproxy拷贝简单引用了一份rbtree的代码，但看下代码，感觉有些疑问，没有时间去仔细探究。
		干脆从linux直接拿来，稍微改编下，更加可靠。不要问我原理哈，自己看。
		http://en.wikipedia.org/wiki/Rbtree
	*/
	class QKCAPI RBNode {
	public:
		RBNode();
		virtual ~RBNode();
		void Link(RBNode * parent, RBNode ** link);

		RBNode * Parent;
		RBNode * Right;
		RBNode * Left;
		int Color;

		inline bool IsBlack() const { return Color == kBlack; }
		inline bool IsRed() const { return Color == kRed; }
		inline void SetBlack() { Color = kBlack; }
		inline void SetRed() { Color = kRed; }

		inline bool IsEmpty() const { return (Parent == this); }
		inline void Clear() { Parent = this; }

		void SetParentColor(RBNode * parent, int color);


		static const int kRed = 0;
		static const int kBlack = 1;
	};

	class QKCAPI RBTree {
	public:
		typedef int(*KeyCompare)(const RBNode * src, const RBNode * dst);

		RBTree();
		RBTree(KeyCompare func);
		virtual ~RBTree();

		bool Insert(RBNode * node);
		void Erase(RBNode * node);
		RBNode * Find(const RBNode * node);

		const RBNode * First() const;
		const RBNode * Last() const;
		const RBNode * Prev(const RBNode * node) const;
		const RBNode * Next(const RBNode * node) const;

		inline const RBNode * Root() const { return root_; }
		inline RBNode * Root() { return root_; }
		inline void CompareFunction(KeyCompare func) { key_compare_ = func; }
	private:
		RBNode * root_;
		KeyCompare key_compare_;

		void RotateSetParents(RBNode * old_node, RBNode * new_node, int color);
		//__rb_change_child
		void ChangeChild(RBNode * old_node, RBNode * new_node, RBNode * parent);
		//__rb_insert
		void InternalInsert(RBNode * node , bool newleft , RBNode ** leftmost);
		//____rb_erase_color
		void InternalEraseColor(RBNode * parent);
		//rb_replace_node
		void ReplaceNode(RBNode * victim, RBNode * new_node);
	};

}

#endif  /** __QUARK_WOBJS_RBTREE_H */
