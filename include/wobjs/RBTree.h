
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
		RBNode(const RBNode& node);
		~RBNode();
		RBNode& operator=(const RBNode& node);
		void Link(RBNode * parent, RBNode ** link);
		void Reset();

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
		void Assign(const RBNode& node);

		static const int kRed = 0;
		static const int kBlack = 1;
	};

	/***
		2019-11-27
		RBTree只管理节点的指针及其关系，并没有管理节点内存的分配和释放，以及节点所包含的内容。
	*/
	class QKCAPI RBTree {
	public:
		RBTree();
		virtual ~RBTree();

		void Clear();
		inline size_t Size() const { return size_; }

	protected :
		bool Insert(RBNode * node);
		void Erase(RBNode * node);
		const RBNode * Find(const RBNode * node) const;

		const RBNode * First() const;
		RBNode * First() ;
		const RBNode * Last() const;
		RBNode * Last();
		const RBNode * Prev(const RBNode * node) const;
		RBNode * Prev(RBNode * node);
		const RBNode * Next(const RBNode * node) const;
		RBNode * Next(RBNode * node);

		inline const RBNode * Root() const { return root_; }
		inline RBNode * Root() { return root_; }
		inline bool Empty() const { return (root_ == NULL); }

		virtual int Compare(const RBNode * src, const RBNode * dst) const;


		RBNode * root_;
		size_t size_;
		void RotateSetParents(RBNode * old_node, RBNode * new_node, int color);
		//__rb_change_child
		void ChangeChild(RBNode * old_node, RBNode * new_node, RBNode * parent);
		//__rb_insert
		void InternalInsert(RBNode * node , bool newleft , RBNode ** leftmost);
		//__rb_erase_augmented
		RBNode * InternalErase(RBNode * node, RBNode** leftmost);
		//____rb_erase_color
		void InternalEraseColor(RBNode * parent);
		//rb_replace_node
		void ReplaceNode(RBNode * victim, RBNode * new_node);
	};

}

#endif  /** __QUARK_WOBJS_RBTREE_H */
