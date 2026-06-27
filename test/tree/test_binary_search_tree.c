/*
 * test_binary_search_tree.c
 *
 * Unit tests for the binary search tree implementation defined in binary_search_tree.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree/binary_search_tree.h"

static void test_binary_search_tree_of_ints(void);

static int compare_ints(const void *left, const void *right);
static void visit_int(const void *data);

int test_binary_search_tree(void)
{
	printf("Test binary search tree...");

	test_binary_search_tree_of_ints();

	puts(" ✅");
	return 0;
}

static void test_binary_search_tree_of_ints(void) 
{
	bstree_t bst = bstree_create(10, sizeof(int), compare_ints);
	int input [] = {3, 1, 4, 2, 5};
	for (int i = 0; i < (int)(sizeof(input) / sizeof(input[0])); ++i)
		bstree_insert(bst, input + i);

	bstree_visit_in_order(bst, bstree_get_root(bst), visit_int);

	int existing = 4;
	assert(bstree_search(bst, &existing) == true);

	// Try to add existing data 
	assert(bstree_size(bst) == 5);
	assert(bstree_insert(bst, &existing) == false);
	assert(bstree_size(bst) == 5);

	int missing = 99;
	assert(bstree_search(bst, &missing) == false);
	assert(bstree_insert(bst, &missing) == true);
	assert(bstree_size(bst) == 6);

	bstree_free(bst);
}


static int compare_ints(const void *left, const void *right)
{
	const int *int_left = (const int *)left;
	const int *int_right = (const int *)right;

	return (*int_left > *int_right) - (*int_left < *int_right);
}

static int expected[] = {1,2,3,4,5};
static int idx = 0;
static void visit_int(const void *data)
{
    int val = *(const int *)data;
    assert(idx < (int)(sizeof(expected)/sizeof(expected[0])) && val == expected[idx++]);
}

