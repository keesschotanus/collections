/*
 * test_chunk.c
 *
 * Unit tests for the chunk allocator implementation defined in chunk.c.
 * 
 * Correctness of the tests heavily depend upon using -fsanitize=address
 * at the compiler and linker level.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "util/chunk.h"

static void test_chunk_basic_allocation(void);
static void test_chunk_null_cases(void);
static void test_chunk_edge_size_cases(void);
static void test_chunk_large_allocation(void);
static void test_chunk_multiple_chunks(void);
static void test_chunk_memory_boundaries(void);

static void verify_chunk_integrity(chunk_t chunk, size_t expected_size);

int test_chunk(void)
{
	printf("Test chunk allocation...");

	test_chunk_basic_allocation();
	test_chunk_null_cases();
	test_chunk_edge_size_cases();
	test_chunk_large_allocation();
	test_chunk_multiple_chunks();
	test_chunk_memory_boundaries();

	puts(" ✅");
	return 0;
}

static void test_chunk_basic_allocation(void)
{
	/* Allocate a chunk for 100 integers */
	chunk_t chunk = chunk_allocate(100, sizeof(int));
	assert(chunk != NULL);

	verify_chunk_integrity(chunk, 100 * sizeof(int));

	assert(chunk->size == 100 * sizeof(int));
	assert(chunk->used == 0);
	assert(chunk->next == NULL);
	assert(chunk->memory != NULL);

	/* Write and read to verify memory is accessible */
	int *int_array = (int *)chunk->memory;
	for (int i = 0; i < 100; i++)
		int_array[i] = i * 42;

	for (int i = 0; i < 100; i++)
		assert(int_array[i] == i * 42);

	chunk_free(chunk);
}

static void test_chunk_null_cases(void)
{
	assert(chunk_allocate(0, sizeof(int)) == NULL);
	assert(chunk_allocate(100, 0) == NULL);
}

static void test_chunk_edge_size_cases(void)
{
	/* Allocate single byte (smallest possible) */
	chunk_t chunk = chunk_allocate(1, 1);
	assert(chunk != NULL);
	assert(chunk->size == 1);
	chunk_free(chunk);
}

static void test_chunk_large_allocation(void)
{
	/* Allocate 1 million bytes in one chunk */
	size_t alloc_size = 1024 * 1024;  /* 1 MB */
	chunk_t chunk = chunk_allocate(alloc_size, 1);
	assert(chunk != NULL);
	assert(chunk->size == alloc_size);

	/* Write pattern to entire allocation */
	unsigned char *bytes = (unsigned char *)chunk->memory;
	for (size_t i = 0; i < alloc_size; i++)
		bytes[i] = (unsigned char)(i % 256);

	/* Verify pattern (catches corruption) */
	for (size_t i = 0; i < alloc_size; i++)
		assert(bytes[i] == (unsigned char)(i % 256));

	chunk_free(chunk);
}

static void test_chunk_multiple_chunks(void)
{
	/* Create and link 10 chunks */
	chunk_t head = chunk_allocate(100, sizeof(int));
	assert(head != NULL);

	chunk_t current = head;
	for (int i = 0; i < 9; i++) {
		chunk_t next = chunk_allocate(100, sizeof(int));
		assert(next != NULL);
		current->next = next;
		current = next;
	}

	/* Write data to each chunk to verify they're accessible */
	current = head;
	int chunk_num = 0;
	while (current != NULL) {
		int *int_array = (int *)current->memory;
		for (int i = 0; i < 100; i++)
			int_array[i] = chunk_num * 1000 + i;
		current = current->next;
		chunk_num++;
	}

	/* Verify data in each chunk */
	current = head;
	chunk_num = 0;
	while (current != NULL) {
		int const *int_array = (int *)current->memory;
		for (int i = 0; i < 100; i++)
			assert(int_array[i] == chunk_num * 1000 + i);
		current = current->next;
		chunk_num++;
	}

	chunk_free(head);
}

static void test_chunk_memory_boundaries(void)
{
	chunk_t chunk1 = chunk_allocate(10, sizeof(int));
	chunk_t chunk2 = chunk_allocate(10, sizeof(int));
	chunk_t chunk3 = chunk_allocate(10, sizeof(int));

	assert(chunk1 != NULL && chunk2 != NULL && chunk3 != NULL);

	/* Verify each chunk has distinct memory */
	assert(chunk1->memory != chunk2->memory);
	assert(chunk2->memory != chunk3->memory);
	assert(chunk1->memory != chunk3->memory);

	/* Write boundary values to each */
	int *mem1 = (int *)chunk1->memory;
	int *mem2 = (int *)chunk2->memory;
	int *mem3 = (int *)chunk3->memory;

	mem1[9] = 111;  /* Last element of chunk1 */
	mem2[0] = 222;  /* First element of chunk2 */
	mem2[9] = 333;  /* Last element of chunk2 */
	mem3[0] = 444;  /* First element of chunk3 */

	/* Verify values weren't corrupted/overwritten */
	assert(mem1[9] == 111);
	assert(mem2[0] == 222);
	assert(mem2[9] == 333);
	assert(mem3[0] == 444);

	chunk_free(chunk1);
	chunk_free(chunk2);
	chunk_free(chunk3);
}

static void verify_chunk_integrity(chunk_t chunk, size_t expected_size)
{
	assert(chunk != NULL);
	assert(chunk->memory != NULL);
	assert(chunk->size == expected_size);
	assert(chunk->used == 0);
	assert(chunk->next == NULL);
}
