/*
 * test_binary_tree.c
 *
 * Unit tests for the binary tree implementation defined in binary_tree.c.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree/binary_tree.h"

static void test_struct_binary_tree(void);

static void visit_struct(const void *data);

int test_binary_tree(void)
{
	printf("Test binary tree...");

	test_struct_binary_tree();

	puts(" ✅");
	return 0;
}

struct token {
	unsigned char type;
	unsigned char value;
};

static char result[10] = {};

static void test_struct_binary_tree()
{
	struct token plus = {.type = 'o', .value = '+'};
	struct token one = {.type = 'l', .value = '1'};
	struct token two = {.type = 'l', .value = '2'};

	bintree_t bintree = bintree_create(3, sizeof(struct token));
	btnode_t root = bintree_create_root(bintree, &plus);

	bintree_insert_left(bintree, root, &one);
	bintree_insert_right(bintree, root, &two);

	bintree_visit_pre_order(bintree, bintree_get_root(bintree), visit_struct);
	bintree_visit_in_order(bintree, bintree_get_root(bintree), visit_struct);
	bintree_visit_post_order(bintree, bintree_get_root(bintree), visit_struct);

	bintree_free(bintree);

	assert(strcmp("+121+212+", result) == 0);
}

static size_t result_idx = 0;

static void visit_struct(const void *data)
{
	struct token token = *(const struct token*)data;
	result[result_idx++] = token.value;
	result[result_idx] = '\0';
}
