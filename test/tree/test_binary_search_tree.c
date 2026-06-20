/*
 * test_binary_search_tree.c
 *
 * Unit tests for the binary search tree implementation defined in binary_search_tree.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree/binary_search_tree.h"

static int compare_ints(const void *a, const void *b);

int test_binary_search_tree(void)
{
	bstree_t int_binary_search_tree = bstree_create(10, sizeof(int), compare_ints);
	int a = 2, b = 1, c= 3, d = -1;
	bstree_insert(int_binary_search_tree, &a);
	bstree_insert(int_binary_search_tree, &b);
	bstree_insert(int_binary_search_tree, &c);
	bstree_insert(int_binary_search_tree, &d);
	bstree_visit_in_order(int_binary_search_tree, bstree_get_root(int_binary_search_tree));

	printf("All binary search tests passed!\n");
	return 0;
}

static int compare_ints(const void *a, const void *b)
{
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;

	return (*int_a > *int_b) - (*int_a < *int_b);
}

