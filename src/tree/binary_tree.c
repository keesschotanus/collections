/**
 * @file binary_tree.c
 * 
 * This file implements a binary tree data structure.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the tree
 * from users of the API.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "tree/binary_tree.h"
#include "util/chunk.h"

#define INITIAL_CAPACITY 32


struct binary_tree
{
	btnode_t root;
	size_t number_of_elements;
	size_t element_size;
	chunk_t chunks;
	size_t initial_capacity;
};

struct binary_tree_node
{
	btnode_t left;
	btnode_t right;
	char data[]; // flexible array member (must be last)
};


static btnode_t allocate_node(bintree_t bintree);

bintree_t bintree_create(size_t initial_capacity, size_t element_size)
{
	if (element_size == 0)
		return NULL;

	struct binary_tree *bintree = malloc(sizeof (struct binary_tree));
	if (bintree == NULL) 
	{
		errno = ENOMEM;
		return NULL;
	}

	bintree->chunks = chunk_allocate(initial_capacity, sizeof(struct binary_tree_node) + element_size);
	if (bintree->chunks == NULL)
	{
		errno = ENOMEM;
		free(bintree);
		return NULL;
	}

	bintree->number_of_elements = 0;
	bintree->initial_capacity = initial_capacity;
	bintree->element_size = element_size;
	bintree->root = NULL;

	return bintree;
}

btnode_t bintree_create_root(bintree_t bintree, const void *element)
{
	if (bintree == NULL || element == NULL)
		return NULL;

	bintree->root = allocate_node(bintree);
	if (bintree->root != NULL) {
		memcpy(bintree->root->data, element, bintree->element_size);
		++bintree->number_of_elements;
	}


	return bintree->root;
}

btnode_t bintree_get_root(bintree_t bintree) {
	return bintree->root;
}


btnode_t bintree_insert_left(bintree_t bintree, btnode_t node, const void *element)
{
	if (bintree == NULL || node == NULL || element == NULL || node->left != NULL)
		return NULL;

	btnode_t new_node = allocate_node(bintree);
	if (new_node != NULL) {
		++bintree->number_of_elements;
		node->left = new_node;
		memcpy(new_node->data, element, bintree->element_size);
	}


	return new_node;
}

btnode_t bintree_insert_right(bintree_t bintree, btnode_t node, const void *element)
{
	if (bintree == NULL || node == NULL || element == NULL || node->right != NULL)
		return NULL;

	btnode_t new_node = allocate_node(bintree);
	if (new_node != NULL) {
		++bintree->number_of_elements;
		node->right = new_node;
		memcpy(new_node->data, element, bintree->element_size);
	}

	return new_node;
}

const void* bintree_get_node_data(btnode_t node)
{
	if (node == NULL)
		return NULL;
	return node->data;
}

void bintree_visit_pre_order(bintree_t bintree, btnode_t node, void (*visit)(const void *))
{
    	if (node != NULL) {
        	visit(node->data);
        	bintree_visit_pre_order(bintree, node->left, visit);
        	bintree_visit_pre_order(bintree, node->right, visit);
	}
}

void bintree_visit_in_order(bintree_t bintree, btnode_t node, void (*visit)(const void *)) {
    	if (node != NULL) {
        	bintree_visit_in_order(bintree, node->left, visit);
        	visit(node->data);
        	bintree_visit_in_order(bintree, node->right, visit);
	}
}


void bintree_visit_post_order(bintree_t bintree, btnode_t node, void (*visit)(const void *)) {
	if (node != NULL) {
        	bintree_visit_post_order(bintree, node->left, visit);
        	bintree_visit_post_order(bintree, node->right, visit);
        	visit(node->data);
	}
}

size_t bintree_size(bintree_t bintree) {
	return bintree->number_of_elements;
}

void bintree_free(bintree_t bintree)
{
	if (bintree == NULL)
		return;

	chunk_free(bintree->chunks);
	free(bintree);
}


static btnode_t allocate_node(bintree_t bintree)
{
	size_t node_size = sizeof(struct binary_tree_node) + bintree->element_size;
	if (bintree->chunks == NULL || bintree->chunks->used + node_size > bintree->chunks->size)
	{
		// allocate new chunk
		chunk_t new_chunk = chunk_allocate(bintree->initial_capacity, node_size);
		if (new_chunk == NULL) {
			errno = ENOMEM;
			return NULL;
		}

		new_chunk->next = bintree->chunks;
		bintree->chunks = new_chunk;
	}
	btnode_t node = (btnode_t)(bintree->chunks->memory + bintree->chunks->used);
	node->left = NULL;
	node->right = NULL;

	bintree->chunks->used += node_size;

	return node;
}
