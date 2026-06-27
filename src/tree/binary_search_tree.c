/**
 * @file binary_tree.c
 * 
 * This file implements a binary search tree data structure.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the tree
 * from users of the API.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "tree/binary_search_tree.h"

struct binary_search_tree
{
	bintree_t tree;
	int (*compare)(const void *, const void *);
};

struct binary_tree_node
{
	btnode_t left;
	btnode_t right;
	char data[]; // flexible array member (must be last)
};

static bool bstree_insert_node(bstree_t bst, btnode_t node, const void *element);

bstree_t bstree_create(size_t initial_capacity, size_t element_size, int (*compare)(const void *, const void *))
{
	if (element_size == 0 || compare == NULL)
		return NULL;

	struct binary_search_tree *bst = malloc(sizeof(struct binary_search_tree));
	if (bst == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	bst->tree = bintree_create(initial_capacity, element_size);
	if (bst->tree == NULL)
	{
		free(bst);
		return NULL;
	}

	bst->compare = compare;
	return bst;
}

btnode_t bstree_get_root(bstree_t bst)
{
	if (bst == NULL)
		return NULL;

	return bintree_get_root(bst->tree);
}

bool bstree_insert(bstree_t bst, const void *element)
{
	if (bst == NULL || element == NULL)
		return false;

	btnode_t root = bintree_get_root(bst->tree);
	if (root == NULL)
		return bintree_create_root(bst->tree, element) != NULL;

	return bstree_insert_node(bst, root, element);
}

btnode_t bstree_search(bstree_t bst, const void *element)
{
	if (bst == NULL || element == NULL)
		return NULL;

	btnode_t node = bintree_get_root(bst->tree);
	while (node != NULL)
	{
		int cmp_result = bst->compare(element, node->data);
		if (cmp_result < 0)
			node = node->left;
		else if (cmp_result > 0)
			node = node->right;
		else
			return node;
	}

	return NULL;
}

const void* bstree_get_node_data(btnode_t node)
{
	if (node == NULL)
		return NULL;
	return node->data;
}

void bstree_visit_pre_order(bstree_t bst, btnode_t node, void (*visit)(const void *))
{
	if (bst == NULL || visit == NULL)
		return;

	bintree_visit_pre_order(bst->tree, node, visit);
}

void bstree_visit_in_order(bstree_t bst, btnode_t node, void (*visit)(const void *))
{
	if (bst == NULL || visit == NULL)
		return;

	bintree_visit_in_order(bst->tree, node, visit);
}

void bstree_visit_post_order(bstree_t bst, btnode_t node, void (*visit)(const void *))
{
	if (bst == NULL || visit == NULL)
		return;

	bintree_visit_post_order(bst->tree, node, visit);
}

size_t bstree_size(bstree_t bst) {
	return bintree_size(bst->tree);
}


void bstree_free(bstree_t bst)
{
	if (bst == NULL)
		return;

	bintree_free(bst->tree);
	free(bst);
}

static bool bstree_insert_node(bstree_t bst, btnode_t node, const void *element)
{
	int cmp_result = bst->compare(element, node->data);
	if (cmp_result < 0)
	{
		if (node->left != NULL)
			return bstree_insert_node(bst, node->left, element);

		btnode_t new_node = bintree_insert_left(bst->tree, node, element);
		return new_node != NULL;
	}
	if (cmp_result > 0)
	{
		if (node->right != NULL)
			return bstree_insert_node(bst, node->right, element);

		btnode_t new_node = bintree_insert_right(bst->tree, node, element);
		return new_node != NULL;
	}

	// Duplicate element - reject it
	return false;
}


