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
	void *data;
	size_t number_of_elements;
	size_t capacity;
	size_t element_size;
};

 bstree bstree_create(size_t initial_capacity, size_t element_size)
{
	if (element_size == 0)
		return NULL;

	return NULL;
}

bool bstree_insert(bstree t, const void *element)
{
	if (t == NULL || element == NULL)
		return false;

	return false;
}

void bstree_free(bstree t)
{
	if (t == NULL)
		return;

	free(t->data);
	free(t);
}

size_t bstree_size(bstree t)
{
	return t ? t->number_of_elements : 0;
}

