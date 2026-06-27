/*
 * test_list.c
 *
 * Unit tests for the list implementation defined in list.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list/list.h"

static void test_list_of_integers(void);
static void test_list_of_doubles(void);
static void test_list_with_invalid_element_size(void);
static void test_list_with_zero_capacity(void);
static void test_list_with_null_operations(void);
static void test_list_with_out_of_bounds_values(void);
static void test_list_visit(void);
static void test_list_clear(void);
static int compare_ints(const void *a, const void *b);

int test_list(void)
{
	printf("Test list (dynamic array)...");

	test_list_of_integers();
	test_list_of_doubles();

	test_list_with_invalid_element_size();
	test_list_with_zero_capacity();
	test_list_with_null_operations();
	test_list_with_out_of_bounds_values();

	test_list_visit();

	test_list_clear();

	puts(" ✅");
	return 0;
}

static void test_list_of_integers(void)
{
	list_t int_list = list_create(10, sizeof(int));
	assert(int_list != NULL);
	assert(list_size(int_list) == 0);

	// Append integers 0 to 49    
	const size_t number_of_elements = 50;    
	for (size_t i = 0; i < number_of_elements; i++)
		list_append(int_list, &i);

	assert(list_size(int_list) == number_of_elements);

	// Verify the values
	for (size_t i = 0; i < number_of_elements; ++i)
	{
		int *val = (int *)list_get(int_list, i);
		assert(val != NULL);
		assert(*val == (int)i);
	}

	// Test set and get
	int new_val = 999;
	assert(list_set(int_list, 10, &new_val));
	int *set_val = (int *)list_get(int_list, 10);
	assert(*set_val == 999);

	list_free(int_list);

	// Test sorting
	list_t unsorted = list_create(10, sizeof(int));
	int values[] = {5, 3, 8, 1, 4};
	for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++)
		list_append(unsorted, &values[i]);
	sort_list(unsorted, compare_ints);
	int expected[] = {1, 3, 4, 5, 8};
	for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); i++)
	{
		int *val = (int *)list_get(unsorted, i);
		assert(val != NULL);
		assert(*val == expected[i]);
	}	
	list_free(unsorted);
}

static void test_list_of_doubles(void)
{
	list_t double_list = list_create(10, sizeof(double));
	assert(double_list != NULL);
	assert(list_size(double_list) == 0);

	const size_t number_of_elements = 50;    
	for (size_t i = 0; i < number_of_elements; i++)
	{
		double d = i * 1.1;
		list_append(double_list, &d);
	}

	assert(list_size(double_list) == number_of_elements);

	for (size_t i = 0; i < number_of_elements; ++i)
	{
		double const *val = (double *)list_get(double_list, i);
		assert(val != NULL);
		assert(*val == i * 1.1);
	}

	list_free(double_list);
}

static void test_list_with_invalid_element_size(void)
{
	list_t invalid = list_create(10, 0);
	assert(invalid == NULL);
}

static void test_list_with_zero_capacity(void)
{
	list_t l = list_create(0, sizeof(int));
	assert(l != NULL);
	assert(list_size(l) == 0);

	int val = 42;
	list_append(l, &val);
	assert(list_size(l) == 1);
	int const *retrieved = (int *)list_get(l, 0);
	assert(retrieved != NULL);
	assert(*retrieved == 42);

	list_free(l);
}

static void test_list_with_null_operations(void)
{
	// Test NULL list operations
	list_append(NULL, NULL);
	assert(list_get(NULL, 0) == NULL);
	assert(list_set(NULL, 0, NULL) == false);
	assert(list_size(NULL) == 0);
	list_free(NULL);
}

static void test_list_with_out_of_bounds_values(void)
{
	// Test out of bounds
	list_t l = list_create(5, sizeof(int));
	assert(l != NULL);
	assert(list_get(l, 0) == NULL); // empty list
	assert(list_set(l, 0, NULL) == false);

	int val = 42;
	list_append(l, &val);
	assert(list_get(l, 1) == NULL); // out of bounds
	assert(list_set(l, 1, &val) == false);

	list_free(l);
}

static double visited_sum = 0;
static void visit_double(const void *data)
{
	visited_sum += *(const double *)data;
}

static void test_list_visit(void)
{
	list_t double_list = list_create(10, sizeof(double));
	list_append(double_list, &(double){2.25});
	list_append(double_list, &(double){4.50});
	list_append(double_list, &(double){6.25});
	list_append(double_list, &(double){8.0});
	list_visit(double_list, visit_double);
	assert(visited_sum == 2.25 + 4.50 + 6.25 + 8.0);

	list_free(double_list);
}

static void test_list_clear(void)
{
	list_t int_list = list_create(10, sizeof(int));
	list_append(int_list, &(int){2});
	list_append(int_list, &(int){4});
	assert(list_size(int_list) == 2);

	list_clear(int_list);
	assert(list_size(int_list) == 0);
	list_append(int_list, &(int){3});
	assert(list_size(int_list) == 1);

	list_free(int_list);
}

static int compare_ints(const void *a, const void *b)
{
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;
	return (*int_a > *int_b) - (*int_a < *int_b);
}