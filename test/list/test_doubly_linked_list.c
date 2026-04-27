/*
 * test_doubly_linked_list.c
 *
 * Unit tests for the doubly linked list implementation defined in doubly_linked_list.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "doubly_linked_list.h"

static void test_list(void);
static void test_list_find(void);
static void test_list_insert(void);
static void test_list_remove(void);

static int compare_ints(const void *a, const void *b);
static dllist create_list_with_elements_2_4_6_8();
static void verify_list_contents(dllist l, const int *expected_values, size_t expected_size);

int test_doubly_linked_list(void)
{
	test_list();
	test_list_find();
	test_list_insert();
	test_list_remove();

	printf("All doubly_linked_list tests passed!\n");
	return 0;
}

static void test_list(void)
{
	dllist int_list = create_list_with_elements_2_4_6_8();

	verify_list_contents(int_list, (int[]){2, 4, 6, 8}, sizeof(int));
	dllist_free(int_list);
}

static void test_list_find(void)
{
	dllist int_list = create_list_with_elements_2_4_6_8();

	for (int i = 2; i <= 8; i += 2)
	{
		dllnode node = dllist_find(int_list, &i, compare_ints);
		assert(node != NULL);
		const void* data = dllist_node_data(node);
		assert(*(int *)data == i);
	}

	// Find a non-existent element
	assert(dllist_find(int_list, &(int){1}, compare_ints) == NULL);

	dllist_free(int_list);
}

static void test_list_insert(void)
{
	dllist int_list = create_list_with_elements_2_4_6_8();

	// Insert element at beginning of list
	dllist_insert_before(int_list, NULL, &(int){1});

	// Insert element at end of list
	dllist_insert_after(int_list, NULL, &(int){10});

	// Find element somewhere in the middle of the list and insert before and after it 
	dllnode node = dllist_find(int_list, &(int){6}, compare_ints);
	assert(node != NULL);
	dllist_insert_before(int_list, node, &(int){5});
	dllist_insert_after(int_list, node, &(int){7});

	int expected_values[] = {1, 2, 4, 5, 6, 7, 8, 10};
	verify_list_contents(int_list, expected_values, sizeof(expected_values) / sizeof(int));

	dllist_free(int_list);
}

static void test_list_remove(void)
{
	dllist int_list = create_list_with_elements_2_4_6_8();

	int remove_elements[] = {6 ,2, 4, 8};
	for (size_t i = 0; i < sizeof(remove_elements) / sizeof(remove_elements[0]); i++)
	{
		int target = remove_elements[i];
		dllnode node = dllist_find(int_list, &target, compare_ints);
		assert(node != NULL);
		assert(dllist_remove(int_list, node));
		node = dllist_find(int_list, &target, compare_ints);
		assert(node == NULL);
	}

	// Remove from an empty list
	assert(dllist_remove(int_list, NULL) == false);

	dllist_free(int_list);
}

static int compare_ints(const void *a, const void *b)
{
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;

	return (*int_a > *int_b) - (*int_a < *int_b);
}

static dllist create_list_with_elements_2_4_6_8() {
	dllist l = dllist_create(sizeof(int));
	assert(l != NULL);

	for (int i = 2; i <= 8; i += 2)
	{
		assert(dllist_insert_after(l, NULL, &i));
	}

	return l;
}

static void verify_list_contents(dllist l, const int *expected_values, size_t expected_size) {
	dllnode node = dllist_first(l);
	size_t idx = 0;
	while (node != NULL)
	{
		const void* data = dllist_node_data(node);
		assert(data != NULL);
		assert(*(int *)data == expected_values[idx]);
		node = dllist_next(l, node);
		idx++;
	}
	assert(idx == expected_size); // Ensure we traversed the expected number of elements
}
