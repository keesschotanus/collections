/*
 * test_binary_search_tree.c
 *
 * Unit tests for the binary search tree implementation defined in binary_search_tree.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree/binary_search_tree.h"

static void test_int_binary_search_tree(void);
static void test_struct_binary_search_tree(void);

static int compare_ints(const void *left, const void *right);
static int compare_structs(const void *left, const void *right);
static void visit_int(const void *data);
static void visit_struct(const void *data);

int test_binary_search_tree(void)
{
	test_int_binary_search_tree();
	test_struct_binary_search_tree();

	printf("All binary search tests passed!\n");
	return 0;
}

static void test_int_binary_search_tree(void) 
{
	bstree_t bst = bstree_create(10, sizeof(int), compare_ints);
	int a = 2, b = 1, c= 3, d = -1;
	bstree_insert(bst, &a);
	bstree_insert(bst, &b);
	bstree_insert(bst, &c);
	bstree_insert(bst, &d);
}

struct token {
	unsigned char type;
	unsigned char value;
};

static void test_struct_binary_search_tree()
{

	struct token plus = {.type = 'o', .value = '+'};
	struct token one = {.type = 'l', .value = '1'};
	struct token two = {.type = 'l', .value = '2'};

	bstree_t bst = bstree_create(3, sizeof(struct token), compare_structs);
	bstree_insert(bst, &plus);
	bstree_insert(bst, &one);
	bstree_insert(bst, &two);

	bstree_visit_pre_order(bst, bstree_get_root(bst), visit_struct);
	puts("");
	bstree_visit_in_order(bst, bstree_get_root(bst), visit_struct);
	puts("");
	bstree_visit_post_order(bst, bstree_get_root(bst), visit_struct);
	puts("");



}

static int compare_ints(const void *left, const void *right)
{
	const int *int_left = (const int *)left;
	const int *int_right = (const int *)right;

	return (*int_left > *int_right) - (*int_left < *int_right);
}

static int compare_structs(const void *left, const void *right)
{
	struct token left_token = *(const struct token*)left;
	struct token right_token = *(const struct token*)right;

	// We want the literal value of '1' to sort before and '2' to sort after '+'
	int result = left_token.type - right_token.type;
	if (result == 0)
		result = left_token.value - right_token.value;
	else {
		if (left_token.type == 'o')
			result = right_token.value == '1' ? 1 : -1;
		else 
			result = left_token.value == '1' ? -1 : 1;
	}		

	return result;
}

static void visit_int(const void *data)
{
	printf("%d ", *(const int *)data);
}

static void visit_struct(const void *data)
{
	struct token token = *(const struct token*)data;

	printf("%c", token.value);
}
