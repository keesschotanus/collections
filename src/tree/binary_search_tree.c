/**
 * @file binary_tree.c
 * 
 * This file implements a binary search tree data structure.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the tree
 * from users of the API.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree/binary_search_tree.h"

#define INITIAL_CAPACITY 32


struct binary_search_tree
{
	bstnode_t root;
	size_t element_size;
	chunk_t chunks;
	struct doubly_linked_list_node *free_list;
	size_t initial_capacity;
	int (*compare)(const void *, const void *);
};

struct binary_search_tree_node
{
	bstnode_t left;
	bstnode_t right;
	char data[]; // flexible array member (must be last)
};


static bstnode_t allocate_node(bstree_t bst);
static void free_node(bstree_t bst, bstnode_t node);

bstree_t bstree_create(size_t initial_capacity, size_t element_size, int (*compare)(const void *, const void *))
{
	if (element_size == 0 || compare == NULL)
		return NULL;

	struct binary_search_tree *bst = malloc(sizeof *bst);
	if (bst == NULL) 
	{
		errno = ENOMEM;
		return NULL;
	}

	bst->chunks = chunk_allocate(initial_capacity, sizeof(struct binary_search_tree_node) + element_size);
	if (bst->chunks == NULL)
	{
		errno = ENOMEM;
		free(bst);
		return NULL;
	}

	bst->compare = compare;
	bst->initial_capacity = initial_capacity;
	bst->element_size = element_size;
	bst->root = NULL;

	return bst;
}

bstnode_t bstree_get_root(bstree_t bst) {
	return bst->root;
}

static bstnode_t bstree_insert_node(bstree_t bst, bstnode_t node, const void *element);

bool bstree_insert(bstree_t bst, const void *element)
{
	if (bst == NULL || element == NULL)
		return false;

	bst->root = bstree_insert_node(bst, bst->root, element);
		
	return true;
}

void bstree_visit_pre_order(bstree_t bst, bstnode_t node, void (*visit)(const void *)) {
    if (node != NULL) {
        	visit(node->data);
        	bstree_visit_pre_order(bst, node->left, visit);
        	bstree_visit_pre_order(bst, node->right, visit);
    }
}

void bstree_visit_in_order(bstree_t bst, bstnode_t node, void (*visit)(const void *)) {
    if (node != NULL) {
        	bstree_visit_in_order(bst, node->left, visit);
        	visit(node->data);
        	bstree_visit_in_order(bst, node->right, visit);
    }
}


void bstree_visit_post_order(bstree_t bst, bstnode_t node, void (*visit)(const void *)) {
    if (node != NULL) {
        	bstree_visit_post_order(bst, node->left, visit);
        	bstree_visit_post_order(bst, node->right, visit);
        	visit(node->data);
    }
}

void bstree_free(bstree_t bst)
{
	if (bst == NULL)
		return;

	// TODO traverse the tree and free all nodes
	free(bst);
}

static bstnode_t bstree_insert_node(bstree_t bst, bstnode_t node, const void *element)
{
    	if (node == NULL) {
		bstnode_t new_node = allocate_node(bst);
		new_node->left = NULL;
		new_node->right = NULL;
		memcpy(new_node->data, element, bst->element_size);
        	return new_node;
    	}

    	int cmp_result = bst->compare(element, node->data);
	if (cmp_result < 0)
		node->left = bstree_insert_node(bst, node->left, element);
	else if (cmp_result > 0)
		node->right = bstree_insert_node(bst, node->right, element);
	else return node;

	return node;
}


static bstnode_t allocate_node(bstree_t bst)
{
	if (bst->free_list != NULL)
	{
		// reuse node from free list
		// TODO
	}
	size_t node_size = sizeof(struct binary_search_tree_node) + bst->element_size;
	if (bst->chunks == NULL || bst->chunks->used + node_size > bst->chunks->size)
	{
		// allocate new chunk
		chunk_t new_chunk = chunk_allocate(bst->initial_capacity, node_size);
		if (new_chunk == NULL) {
			errno = ENOMEM;
			return NULL;
		}

		new_chunk->next = bst->chunks;
		bst->chunks = new_chunk;
	}
	bstnode_t node = (bstnode_t)(bst->chunks->memory + bst->chunks->used);
	bst->chunks->used += node_size;

	return node;
}
