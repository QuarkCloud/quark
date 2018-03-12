
#include "rbtree.h"

static void __rb_rotate_left(rb_node_t *node, rb_root_t *root)
{
	rb_node_t *right = node->right;

	if ((node->right = right->left))
		right->left->parent = node;
	right->left = node;

	if ((right->parent = node->parent))
	{
		if (node == node->parent->left)
			node->parent->left = right;
		else
			node->parent->right = right;
	}
	else
		root->root = right;
	node->parent = right;
}

static void __rb_rotate_right(rb_node_t *node, rb_root_t *root)
{
	rb_node_t *left = node->left;

	if ((node->left = left->right))
		left->right->parent = node;
	left->right = node;

	if ((left->parent = node->parent))
	{
		if (node == node->parent->right)
			node->parent->right = left;
		else
			node->parent->left = left;
	}
	else
		root->root = left;
	node->parent = left;
}

void rb_insert_color(rb_node_t *node, rb_root_t *root)
{
	rb_node_t *parent, *gparent;

	while ((parent = node->parent) && parent->color == RB_RED)
	{
		gparent = parent->parent;

		if (parent == gparent->left)
		{
			{
				register rb_node_t *uncle = gparent->right;
				if (uncle && uncle->color == RB_RED)
				{
					uncle->color = RB_BLACK;
					parent->color = RB_BLACK;
					gparent->color = RB_RED;
					node = gparent;
					continue;
				}
			}

			if (parent->right == node)
			{
				register rb_node_t *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			parent->color = RB_BLACK;
			gparent->color = RB_RED;
			__rb_rotate_right(gparent, root);
		} else {
			{
				register rb_node_t *uncle = gparent->left;
				if (uncle && uncle->color == RB_RED)
				{
					uncle->color = RB_BLACK;
					parent->color = RB_BLACK;
					gparent->color = RB_RED;
					node = gparent;
					continue;
				}
			}

			if (parent->left == node)
			{
				register rb_node_t *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			parent->color = RB_BLACK;
			gparent->color = RB_RED;
			__rb_rotate_left(gparent, root);
		}
	}

	root->root->color = RB_BLACK;
}

static void __rb_erase_color(rb_node_t *node, rb_node_t *parent,
			     rb_root_t *root)
{
	rb_node_t *other;

	while ((!node || node->color == RB_BLACK) && node != root->root)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (other->color == RB_RED)
			{
				other->color = RB_BLACK;
				parent->color = RB_RED;
				__rb_rotate_left(parent, root);
				other = parent->right;
			}
			if ((!other->left ||
			     other->left->color == RB_BLACK)
			    && (!other->right ||
				other->right->color == RB_BLACK))
			{
				other->color = RB_RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->right ||
				    other->right->color == RB_BLACK)
				{
					register rb_node_t *o_left;
					if ((o_left = other->left))
						o_left->color = RB_BLACK;
					other->color = RB_RED;
					__rb_rotate_right(other, root);
					other = parent->right;
				}
				other->color = parent->color;
				parent->color = RB_BLACK;
				if (other->right)
					other->right->color = RB_BLACK;
				__rb_rotate_left(parent, root);
				node = root->root;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (other->color == RB_RED)
			{
				other->color = RB_BLACK;
				parent->color = RB_RED;
				__rb_rotate_right(parent, root);
				other = parent->left;
			}
			if ((!other->left ||
			     other->left->color == RB_BLACK)
			    && (!other->right ||
				other->right->color == RB_BLACK))
			{
				other->color = RB_RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->left ||
				    other->left->color == RB_BLACK)
				{
					register rb_node_t *o_right;
					if ((o_right = other->right))
						o_right->color = RB_BLACK;
					other->color = RB_RED;
					__rb_rotate_left(other, root);
					other = parent->left;
				}
				other->color = parent->color;
				parent->color = RB_BLACK;
				if (other->left)
					other->left->color = RB_BLACK;
				__rb_rotate_right(parent, root);
				node = root->root;
				break;
			}
		}
	}
	if (node)
		node->color = RB_BLACK;
}

void rb_erase(rb_node_t *node, rb_root_t *root)
{
	rb_node_t *child, *parent;
	int color;

	if (!node->left)
		child = node->right;
	else if (!node->right)
		child = node->left;
	else
	{
		rb_node_t *old = node, *left;

		node = node->right;
		while ((left = node->left) != NULL)
			node = left;
		child = node->right;
		parent = node->parent;
		color = node->color;

		if (child)
			child->parent = parent;
		if (parent)
		{
			if (parent->left == node)
				parent->left = child;
			else
				parent->right = child;
		}
		else
			root->root = child;

		if (node->parent == old)
			parent = node;
		node->parent = old->parent;
		node->color = old->color;
		node->right = old->right;
		node->left = old->left;

		if (old->parent)
		{
			if (old->parent->left == old)
				old->parent->left = node;
			else
				old->parent->right = node;
		} else
			root->root = node;

		old->left->parent = node;
		if (old->right)
			old->right->parent = node;
		goto color;
	}

	parent = node->parent;
	color = node->color;

	if (child)
		child->parent = parent;
	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root->root = child;

 color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}
/*
 * This function returns the first node (in sort order) of the tree.
 */
rb_node_t *rb_first(rb_root_t *root)
{
	rb_node_t	*n;

	n = root->root;
	if (!n)
		return NULL;
	while (n->left)
		n = n->left;
	return n;
}
// EXPORT_SYMBOL(rb_first);

rb_node_t *rb_last(rb_root_t *root)
{
	rb_node_t	*n;

	n = root->root;
	if (!n)
		return NULL;
	while (n->right)
		n = n->right;
	return n;
}
// EXPORT_SYMBOL(rb_last);

rb_node_t *rb_next(rb_node_t *node)
{
	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->right) {
		node = node->right;
		while (node->left)
			node=node->left;
		return node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while (node->parent && node == node->parent->right)
		node = node->parent;

	return node->parent;
}
// EXPORT_SYMBOL(rb_next);

rb_node_t *rb_prev(rb_node_t *node)
{
	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->left) {
		node = node->left;
		while (node->right)
			node=node->right;
		return node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while (node->parent && node == node->parent->left)
		node = node->parent;

	return node->parent;
}
// EXPORT_SYMBOL(rb_prev);

void rb_replace_node(rb_node_t *victim, rb_node_t *new_node,   rb_root_t *root)
{
	rb_node_t *parent = victim->parent;

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->left)
			parent->left = new_node;
		else
			parent->right = new_node;
	} else {
		root->root = new_node;
	}
	if (victim->left)
		victim->left->parent = new_node;
	if (victim->right)
		victim->right->parent = new_node;

	/* Copy the pointers/colour from the victim to the replacement */
	*new_node = *victim;
}
// EXPORT_SYMBOL(rb_replace_node);
