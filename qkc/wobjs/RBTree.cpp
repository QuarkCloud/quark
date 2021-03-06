
#include "wobjs/RBTree.h"

namespace qkc {

	RBNode::RBNode()
	{
		Reset();
	}

	RBNode::RBNode(const RBNode& node)
	{
		Assign(node);
	}	

	RBNode::~RBNode()
	{
		//
	}
	RBNode& RBNode::operator=(const RBNode& node)
	{
		Assign(node);
		return (*this);
	}

	void RBNode::Link(RBNode * parent, RBNode ** link)
	{
		Parent = parent;
		Color = kRed;
		Right = Left = NULL;

		if (link != NULL)
			*link = this;
	}

	void RBNode::Reset()
	{
		Parent = Right = Left = NULL;
		Color = kRed;
	}

	void RBNode::SetParentColor(RBNode * parent, int color)
	{
		Parent = parent;
		Color = color;
	}

	void RBNode::Assign(const RBNode& node)
	{
		Parent = node.Parent;
		Right = node.Right;
		Left = node.Left;
		Color = node.Color;
	}

	RBTree::RBTree()
	{
		root_ = NULL;
		size_ = 0;
	}

	RBTree::~RBTree()
	{
		//
	}

	void RBTree::Clear()
	{
		root_ = NULL;
		size_ = 0;
	}

	int RBTree::Compare(const RBNode * src, const RBNode * dst) const
	{
		intptr_t diff = (intptr_t)src - (intptr_t)dst;
		if (diff == 0)
			return 0;
		if (diff < 0)
			return -1;
		else
			return 1;
	}

	bool RBTree::Insert(RBNode * node)
	{
		RBNode ** link = &root_, *parent = NULL;

		while ((*link) != NULL)
		{
			parent = *link;
			int value = Compare(node , parent);//node->Compare(parent);
			if (value < 0)
				*link = parent->Left;
			else if (value > 0)
				*link = parent->Right;
			else
			{
				//key相等，那就替换掉原来的节点
				RBNode * gparent = parent->Parent;
				node->Parent = gparent;
				node->Color = parent->Color;
				node->Left = parent->Left;
				node->Right = parent->Right;

				if (gparent == NULL)
				{
					if (root_ == parent)
						root_ = node;
					else
						return false;
				}
				else
				{
					if (gparent->Left == parent)
						gparent->Left = node;
					else if (gparent->Right == parent)
						gparent->Right = node;
					else
						return false;
				}
			}
		}

		node->Link(parent, link);
		InternalInsert(node, false , NULL);
		++size_;
		return true;
	}

	void RBTree::Erase(RBNode * node)
	{
		RBNode * rebalance = InternalErase(node, NULL);
		if (rebalance != NULL)
			InternalEraseColor(rebalance);
		--size_;
	}

	const RBNode * RBTree::Find(const RBNode * node) const
	{
		const RBNode * cur = root_;
		while (cur != NULL)
		{
			int value = Compare(node, cur);
			if (value == 0)
				break;
			if (value < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		return cur;
	}

	void RBTree::RotateSetParents(RBNode * old_node, RBNode * new_node, int color)
	{
		RBNode * parent = old_node->Parent;
		new_node->Parent = old_node->Parent;
		new_node->Color = old_node->Color;

		old_node->SetParentColor(new_node, color);
		ChangeChild(old_node, new_node, parent);
	}


	void RBTree::ChangeChild(RBNode * old_node, RBNode * new_node, RBNode * parent)
	{
		if (parent != NULL)
		{
			if (parent->Left == old_node)
				parent->Left = new_node;
			else
				parent->Right = new_node;
		}
		else
			root_ = new_node;
	}

	void RBTree::InternalInsert(RBNode * node, bool newleft, RBNode ** leftmost)
	{
		RBNode * parent = node->Parent , *gparent , * tmp;
		if (newleft)
			*leftmost = node;
		while (true)
		{
			if (parent == NULL && node->IsRed())
			{
				node->SetParentColor(NULL, RBNode::kBlack);
				break;
			}

			if (parent->Color == RBNode::kBlack)
				break;

			gparent = parent->Parent;
			tmp = gparent->Right;
			if (parent != tmp) //parent == gparent->Left
			{
				if (tmp && tmp->IsRed())
				{
					/*
					 * Case 1 - node's uncle is red (color flips).
					 *
					 *       G            g
					 *      / \          / \
					 *     p   u  -->   P   U
					 *    /            /
					 *   n            n
					 *
					 * However, since g's parent might be red, and
					 * 4) does not allow this, we need to recurse
					 * at g.
					 */
					tmp->SetParentColor(gparent, RBNode::kBlack);
					parent->SetParentColor(gparent, RBNode::kBlack);
					node = gparent;
					parent = node->Parent;
					node->SetParentColor(parent, RBNode::kRed);
					continue;
				}

				tmp = parent->Right;
				if (node == tmp)
				{
					/*
					 * Case 2 - node's uncle is black and node is
					 * the parent's right child (left rotate at parent).
					 *
					 *      G             G
					 *     / \           / \
					 *    p   U  -->    n   U
					 *     \           /
					 *      n         p
					 *
					 * This still leaves us in violation of 4), the
					 * continuation into Case 3 will fix that.
					 */
					tmp = node->Left;
					parent->Right = tmp;
					node->Left = parent;

					if (tmp != NULL)
						tmp->SetParentColor(parent, RBNode::kBlack);

					parent->SetParentColor(node, RBNode::kRed);
					//augment_rotate(parent , node) ;
					parent = node;
					tmp = node->Right;
				}

				/*
				 * Case 3 - node's uncle is black and node is
				 * the parent's left child (right rotate at gparent).
				 *
				 *        G           P
				 *       / \         / \
				 *      p   U  -->  n   g
				 *     /                 \
				 *    n                   U
				 */
				gparent->Left = tmp;
				parent->Right = gparent;
				if (tmp != NULL)
					tmp->SetParentColor(gparent, RBNode::kBlack);
				RotateSetParents(gparent, parent, RBNode::kRed);
				//augment_rotate(gparent, parent);
				break;
			}
			else
			{
				tmp = gparent->Left;
				if (tmp && tmp->IsRed())
				{
					//case 1 -- color flips
					tmp->SetParentColor(gparent, RBNode::kBlack);
					parent->SetParentColor(gparent, RBNode::kBlack);
					node = gparent;
					parent = node->Parent;
					node->SetParentColor(parent, RBNode::kRed);
					continue;
				}

				tmp = parent->Left;
				if (node == tmp)
				{
					//case 2 -- right rotate at parent 
					tmp = node->Right;
					parent->Left = tmp;
					node->Right = parent;
					if (tmp != NULL)
						tmp->SetParentColor(parent, RBNode::kBlack);
					//augment_rotate(parent , node) ;
					parent = node;
					tmp = node->Left;
				}

				//Case 3 - left rotate at gparent
				gparent->Right = tmp; //==parent->left ;
				parent->Left = gparent;
				if (tmp != NULL)
					tmp->SetParentColor(gparent, RBNode::kBlack);
				RotateSetParents(gparent, parent, RBNode::kRed);
				//augment_rotate(gparent , parent) ;
				break;
			}
		}
	}

	RBNode * RBTree::InternalErase(RBNode * node, RBNode** leftmost)
	{
		RBNode * child = node->Right;
		RBNode * tmp = node->Left;
		RBNode * parent = NULL, *rebalance = NULL;
		RBNode * tp = NULL;
		int tc = 0;

		if (leftmost != NULL && node == *leftmost)
			*leftmost = Next(node);

		if (!tmp) {
			/*
			 * Case 1: node to erase has no more than 1 child (easy!)
			 *
			 * Note that if there is one child it must be red due to 5)
			 * and node must be black due to 4). We adjust colors locally
			 * so as to bypass __rb_erase_color() later on.
			 */
			parent = tp = node->Parent;
			tc = node->Color;
			ChangeChild(node, child, parent);
			if (child != NULL)
			{
				child->Parent = tp;
				child->Color = tc;
				rebalance = NULL;
			}
			else
			{
				if (tc == RBNode::kBlack)
					rebalance = parent;
				else
					rebalance = NULL;
			}

			tmp = parent;
		}
		else if (!child) {
			/* Still case 1, but this time the child is node->rb_left */
			tp = node->Parent;
			tc = node->Color;

			parent = tmp->Parent = tp;
			tmp->Color = tc;
			ChangeChild(node, tmp, parent);
			rebalance = NULL;
			tmp = parent;
		}
		else {
			RBNode * successor = child, *child2 = NULL;
			tmp = child->Left;

			if (!tmp) {
				/*
				 * Case 2: node's successor is its right child
				 *
				 *    (n)          (s)
				 *    / \          / \
				 *  (x) (s)  ->  (x) (c)
				 *        \
				 *        (c)
				 */
				parent = successor;
				child2 = successor->Right;
			}
			else {
				/*
				 * Case 3: node's successor is leftmost under
				 * node's right child subtree
				 *
				 *    (n)          (s)
				 *    / \          / \
				 *  (x) (y)  ->  (x) (y)
				 *      /            /
				 *    (p)          (p)
				 *    /            /
				 *  (s)          (c)
				 *    \
				 *    (c)
				 */
				do {
					parent = successor;
					successor = tmp;
					tmp = tmp->Left;
				} while (tmp);
				child2 = successor->Right;
				parent->Left = child2;
				successor->Right = child;
				child->Parent = successor;
			}

			tmp = node->Left;
			successor->Left = tmp;
			tmp->Parent = successor;

			tmp = tp = node->Parent;
			tc = node->Color;
			ChangeChild(node, successor, tmp);

			if (child2 != NULL)
			{
				successor->Parent = tp;
				successor->Color = tc;

				child2->Parent = parent;
				child2->Color = RBNode::kBlack;
				rebalance = NULL;
			}
			else
			{
				RBNode * tp2 = successor->Parent;
				int tc2 = successor->Color;

				successor->Parent = tp;
				successor->Color = tc;
				if (tc2 == RBNode::kBlack)
					rebalance = parent;
				else
					rebalance = NULL;
			}

			tmp = successor;
		}
		return rebalance;
	}

	void RBTree::InternalEraseColor(RBNode * parent)
	{
		RBNode * node = NULL, *sibling, *tmp1, *tmp2;
		while (true)
		{
			sibling = parent->Right;
			if (node != sibling) // node == parent->Right ;
			{
				if (sibling->IsRed())
				{
					/*
					 * Case 1 - left rotate at parent
					 *
					 *     P               S
					 *    / \             / \
					 *   N   s    -->    p   Sr
					 *      / \         / \
					 *     Sl  Sr      N   Sl
					 */
					tmp1 = sibling->Left;
					parent->Right = tmp1;
					sibling->Left = parent;
					tmp1->SetParentColor(parent, RBNode::kBlack);

					RotateSetParents(parent, sibling, RBNode::kRed);
					//augment_rotate(parent , sibling) ;
					sibling = tmp1;
				}

				tmp1 = sibling->Right;
				if (tmp1 == NULL || tmp1->IsBlack())
				{
					tmp2 = sibling->Left;
					if (tmp2 == NULL || tmp2->IsBlack())
					{
						/*
						 * Case 2 - sibling color flip
						 * (p could be either color here)
						 *
						 *    (p)           (p)
						 *    / \           / \
						 *   N   S    -->  N   s
						 *      / \           / \
						 *     Sl  Sr        Sl  Sr
						 *
						 * This leaves us violating 5) which
						 * can be fixed by flipping p to black
						 * if it was red, or by recursing at p.
						 * p is red when coming from Case 1.
						 */
						sibling->SetParentColor(parent, RBNode::kRed);
						if (parent->IsRed())
							parent->Color = RBNode::kBlack;
						else
						{
							node = parent;
							parent = node->Parent;
							if (parent != NULL)
								continue;
						}
						break;
					}
					/*
					 * Case 3 - right rotate at sibling
					 * (p could be either color here)
					 *
					 *   (p)           (p)
					 *   / \           / \
					 *  N   S    -->  N   sl
					 *     / \             \
					 *    sl  Sr            S
					 *                       \
					 *                        Sr
					 *
					 * Note: p might be red, and then both
					 * p and sl are red after rotation(which
					 * breaks property 4). This is fixed in
					 * Case 4 (in __rb_rotate_set_parents()
					 *         which set sl the color of p
					 *         and set p RB_BLACK)
					 *
					 *   (p)            (sl)
					 *   / \            /  \
					 *  N   sl   -->   P    S
					 *       \        /      \
					 *        S      N        Sr
					 *         \
					 *          Sr
					 */
					tmp1 = tmp2->Right;
					sibling->Left = tmp1;
					tmp2->Right = sibling;
					parent->Right = tmp2;

					if (tmp1 != NULL)
						tmp1->SetParentColor(sibling, RBNode::kBlack);
					//augment_rotate(sibling ,  tmp2) ;
					tmp1 = sibling;
					sibling = tmp2;
				}

				/*
				 * Case 4 - left rotate at parent + color flips
				 * (p and sl could be either color here.
				 *  After rotation, p becomes black, s acquires
				 *  p's color, and sl keeps its color)
				 *
				 *      (p)             (s)
				 *      / \             / \
				 *     N   S     -->   P   Sr
				 *        / \         / \
				 *      (sl) sr      N  (sl)
				 */
				tmp2 = sibling->Left;
				parent->Right = tmp2;
				sibling->Left = parent;
				tmp1->SetParentColor(sibling, RBNode::kBlack);

				if (tmp2 != NULL)
					tmp2->Parent = parent;
				RotateSetParents(parent, sibling, RBNode::kBlack);
				//augment rotate(parent , sibling) ;
				break;
			}
			else
			{
				sibling = parent->Left;
				if (sibling->IsRed() == true)
				{
					//Case 1 -- right rotate at parent 
					tmp1 = sibling->Right;
					parent->Left = tmp1;
					sibling->Right = parent;
					tmp1->SetParentColor(parent, RBNode::kBlack);
					RotateSetParents(parent, sibling, RBNode::kRed);
					//augment rotate(parent , sibling) ;
					sibling = tmp1;
				}

				tmp1 = sibling->Left;
				if (tmp1 == NULL || tmp1->IsBlack() == true)
				{
					tmp2 = sibling->Right;
					if (tmp2 == NULL || tmp2->IsBlack() == true)
					{
						//Case 2 - sibling color flip
						sibling->SetParentColor(parent, RBNode::kRed);
						if (parent->IsRed() == true)
							parent->Color = RBNode::kBlack;
						else
						{
							node = parent;
							parent = node->Parent;
							if (parent != NULL)
								continue;
						}
						break;
					}

					//Case 3 - left rotate at sibling
					tmp1 = tmp2->Left;
					sibling->Right = tmp1;
					tmp2->Left = sibling;
					parent->Left = tmp2;
					if (tmp1 != NULL)
						tmp1->SetParentColor(sibling, RBNode::kBlack);
					RotateSetParents(parent, sibling, RBNode::kBlack);
					//augment_rotate(parent , sibling) ;
					break;
				}
			}
		}	
	}

	const RBNode * RBTree::First() const
	{
		const RBNode * n;
		n = root_;
		if (n == NULL)
			return NULL;
		while (n->Left != NULL)
			n = n->Left;
		return n;
	}

	RBNode * RBTree::First()
	{
		RBNode * n;
		n = root_;
		if (n == NULL)
			return NULL;
		while (n->Left != NULL)
			n = n->Left;
		return n;
	}

	const RBNode * RBTree::Last() const
	{
		const RBNode * n;
		n = root_;
		if (n == NULL)
			return NULL;
		while (n->Right != NULL)
			n = n->Right;
		return n ;	
	}

	RBNode * RBTree::Last() 
	{
		RBNode * n;
		n = root_;
		if (n == NULL)
			return NULL;
		while (n->Right != NULL)
			n = n->Right;
		return n;
	}

	const RBNode * RBTree::Prev(const RBNode * node) const
	{
		RBNode * parent;
		if (node->IsEmpty() == true)
			return NULL;

		//If we have a left-hand child, go down and then right as far as we can.
		if (node->Left != NULL)
		{
			node = node->Left;
			while (node->Right != NULL)
				node = node->Right;
			return node;
		}

		while ((parent = node->Parent) && node == parent->Left)
			node = parent;
		return parent;	
	}
	RBNode * RBTree::Prev(RBNode * node)
	{
		RBNode * parent;
		if (node->IsEmpty() == true)
			return NULL;

		//If we have a left-hand child, go down and then right as far as we can.
		if (node->Left != NULL)
		{
			node = node->Left;
			while (node->Right != NULL)
				node = node->Right;
			return node;
		}

		while ((parent = node->Parent) && node == parent->Left)
			node = parent;
		return parent;
	}


	const RBNode * RBTree::Next(const RBNode * node) const
	{
		RBNode * parent;
		if (node->IsEmpty() == true)
			return NULL;
		//If we have a right-hand child, go down and then left as far as we can.
		if (node->Right != NULL)
		{
			node = node->Right;
			while (node->Left != NULL)
				node = node->Left;
			return node;
		}

		while ((parent = node->Parent) && node == parent->Right)
			node = parent;	

		return parent;
	}

	RBNode * RBTree::Next(RBNode * node) 
	{
		RBNode * parent;
		if (node->IsEmpty() == true)
			return NULL;
		//If we have a right-hand child, go down and then left as far as we can.
		if (node->Right != NULL)
		{
			node = node->Right;
			while (node->Left != NULL)
				node = node->Left;
			return node;
		}

		while ((parent = node->Parent) && node == parent->Right)
			node = parent;

		return parent;
	}


	void RBTree::ReplaceNode(RBNode * victim, RBNode * new_node)
	{
		RBNode * parent = victim->Parent;
		*new_node = *victim;
		if (victim->Left != NULL)
			victim->Left->Parent = new_node;
		if (victim->Right != NULL)
			victim->Right->Parent = new_node;
		ChangeChild(victim, new_node, parent);
	}
}
