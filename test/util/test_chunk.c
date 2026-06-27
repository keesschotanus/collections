/*
 * test_chunk.c
 *
 * Unit tests for the chunk allocator implementation defined in chunk.c.
 * 
 * Memory Leak Detection Strategies:
 * 1. Valgrind: Run with `valgrind --leak-check=full ./build/unittest`
 *    Valgrind will detect any unreleased memory as "definitely lost" or "indirectly lost"
 * 
 * 2. Manual Tracking: Count allocations vs deallocations
 *    Each test verifies that all allocated memory is properly freed by calling chunk_free()
 * 
 * 3. Stress Testing: Large allocations reveal accumulation issues
 *    If a leak exists, it becomes apparent at scale (e.g., 1M allocations)
 * 
 * 4. Edge Case Testing: Tests boundary conditions that might leak
 *    NULL returns, zero values, multiple chunks
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "util/chunk.h"

/* Forward declarations of test functions */
static void test_chunk_basic_allocation(void);
static void test_chunk_null_cases(void);
static void test_chunk_zero_size_cases(void);
static void test_chunk_large_allocation(void);
static void test_chunk_multiple_chunks(void);
static void test_chunk_memory_boundaries(void);
static void test_chunk_stress_allocation(void);

/* Helper function to verify chunk structure integrity */
static void verify_chunk_integrity(chunk_t chunk, size_t expected_size);

int test_chunk(void)
{
	test_chunk_basic_allocation();
	test_chunk_null_cases();
	test_chunk_zero_size_cases();
	test_chunk_large_allocation();
	test_chunk_multiple_chunks();
	test_chunk_memory_boundaries();
	test_chunk_stress_allocation();

	printf("All chunk allocator tests passed!\n");
	return 0;
}

/**
 * TEST 1: Basic Allocation and Deallocation
 * 
 * Tests that:
 * - Allocation creates valid chunk with correct fields
 * - Memory is properly freed with no leaks
 * 
 * Memory Leak Detection Method:
 * Run with valgrind to verify chunk_free() properly releases both:
 *   1. The metadata (chunk_t structure)
 *   2. The allocated memory (chunk->memory)
 */
static void test_chunk_basic_allocation(void)
{
	printf("Testing basic chunk allocation...");

	/* Allocate a chunk for 100 integers */
	chunk_t chunk = chunk_allocate(100, sizeof(int));
	assert(chunk != NULL);

	/* Verify structure integrity */
	verify_chunk_integrity(chunk, 100 * sizeof(int));

	/* Verify fields are initialized correctly */
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

	/* Free the chunk - valgrind will verify both pointers are freed */
	chunk_free(chunk);

	printf(" PASSED\n");
}

/**
 * TEST 2: NULL Pointer Handling
 * 
 * Tests that:
 * - chunk_free(NULL) doesn't crash (defensive programming)
 * - chunk_allocate(0, size) returns NULL safely
 * 
 * Memory Leak Detection Method:
 * No allocation occurs, so nothing to leak. Test verifies defensive code paths.
 */
static void test_chunk_null_cases(void)
{
	printf("Testing NULL pointer handling...");

	/* chunk_free(NULL) should be a no-op */
	chunk_free(NULL);

	/* Allocation with zero nodes should return NULL */
	chunk_t chunk = chunk_allocate(0, sizeof(int));
	assert(chunk == NULL);

	/* Allocation with zero size should return NULL */
	chunk = chunk_allocate(100, 0);
	assert(chunk == NULL);

	printf(" PASSED\n");
}

/**
 * TEST 3: Edge Cases with Zero/Small Values
 * 
 * Tests that:
 * - Allocations work correctly with size_t edge cases
 * - Small allocations don't cause issues
 * 
 * Memory Leak Detection Method:
 * Proper cleanup verifies even small allocations are handled correctly.
 */
static void test_chunk_zero_size_cases(void)
{
	printf("Testing zero/small size cases...");

	/* Allocate single byte (smallest possible) */
	chunk_t chunk = chunk_allocate(1, 1);
	assert(chunk != NULL);
	assert(chunk->size == 1);
	chunk_free(chunk);

	/* Allocate single large node */
	chunk = chunk_allocate(1, 1024 * 1024);  /* 1 MB */
	assert(chunk != NULL);
	assert(chunk->size == 1024 * 1024);
	chunk_free(chunk);

	printf(" PASSED\n");
}

/**
 * TEST 4: Large Single Allocation
 * 
 * Tests that:
 * - Large allocations succeed (if memory available)
 * - Large allocations are properly freed
 * - All memory can be accessed without corruption
 * 
 * Memory Leak Detection Method:
 * Valgrind will report "definitely lost" if chunk_free() doesn't properly
 * free the large allocated block. The test exercises the entire allocated
 * region to detect uninitialized reads/writes.
 */
static void test_chunk_large_allocation(void)
{
	printf("Testing large allocation (1MB)...");

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

	/* Free - valgrind will verify the entire 1MB is freed */
	chunk_free(chunk);

	printf(" PASSED\n");
}

/**
 * TEST 5: Multiple Linked Chunks
 * 
 * Tests that:
 * - Multiple chunks can be linked via ->next
 * - All chunks in chain are freed with single chunk_free() call
 * - No leaks occur with multi-chunk structures
 * 
 * Memory Leak Detection Method:
 * This is CRITICAL: chunk_free() must traverse the entire linked chain
 * and free ALL chunks. Valgrind will report "definitely lost" if any
 * chunk in the chain is not freed.
 * 
 * Manual verification: We create 10 chunks and link them. If chunk_free()
 * doesn't traverse the chain properly, those 9 non-head chunks will leak.
 */
static void test_chunk_multiple_chunks(void)
{
	printf("Testing multiple linked chunks...");

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
		int *int_array = (int *)current->memory;
		for (int i = 0; i < 100; i++)
			assert(int_array[i] == chunk_num * 1000 + i);
		current = current->next;
		chunk_num++;
	}

	/* 
	 * FREE: This must free ALL 10 chunks (head + 9 linked chunks).
	 * If chunk_free() doesn't traverse the ->next chain, the 9 linked
	 * chunks will leak. Valgrind will catch this.
	 */
	chunk_free(head);

	printf(" PASSED\n");
}

/**
 * TEST 6: Memory Boundaries and Overlap Detection
 * 
 * Tests that:
 * - Each chunk gets distinct memory (no overlaps)
 * - Boundary writes don't affect adjacent allocations
 * - Memory layout is correct
 * 
 * Memory Leak Detection Method:
 * Verifies proper allocation. Doesn't directly detect leaks but ensures
 * allocator doesn't return overlapping regions (which would cause
 * corruption and make leak detection unreliable).
 */
static void test_chunk_memory_boundaries(void)
{
	printf("Testing memory boundaries...");

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

	/* Free all - each chunk must free its own memory */
	chunk_free(chunk1);
	chunk_free(chunk2);
	chunk_free(chunk3);

	printf(" PASSED\n");
}

/**
 * TEST 7: Stress Test - Scale Up Leak Detection
 * 
 * Tests that:
 * - Allocations work correctly at scale
 * - No gradual leak under repeated allocation/deallocation
 * - System remains stable with many allocations
 * 
 * Memory Leak Detection Method:
 * THIS IS THE MOST IMPORTANT TEST FOR LEAK DETECTION:
 * 
 * If there's a leak of even a small amount (e.g., 100 bytes per chunk),
 * this test will amplify it. With 10,000 allocations:
 *   - 100 byte leak × 10,000 = 1MB loss (very visible)
 *   - Valgrind will report: "100 byte leak in 10000 blocks"
 * 
 * This catches:
 * - Forgotten free() calls
 * - Partial cleanup (e.g., freeing metadata but not memory)
 * - Off-by-one errors in chunk lists
 * 
 * Run this with: valgrind --leak-check=full ./build/unittest
 * Valgrind summary will show total bytes definitely/indirectly lost.
 */
static void test_chunk_stress_allocation(void)
{
	printf("Testing stress allocation (10,000 chunks)...");

	/* Allocate 10,000 chunks */
	chunk_t *chunks = (chunk_t *)malloc(10000 * sizeof(chunk_t));
	assert(chunks != NULL);

	for (int i = 0; i < 10000; i++) {
		chunks[i] = chunk_allocate(50, sizeof(int));
		assert(chunks[i] != NULL);
		
		/* Verify memory is accessible */
		int *ptr = (int *)chunks[i]->memory;
		ptr[0] = i;
	}

	/* Verify all chunks still have their data */
	for (int i = 0; i < 10000; i++) {
		int *ptr = (int *)chunks[i]->memory;
		assert(ptr[0] == i);
	}

	/* Free all chunks - this must properly free all 10,000 */
	for (int i = 0; i < 10000; i++)
		chunk_free(chunks[i]);

	free(chunks);

	printf(" PASSED\n");
}

/**
 * Helper: Verify Chunk Integrity
 * 
 * Ensures the chunk structure is well-formed and safe to use.
 */
static void verify_chunk_integrity(chunk_t chunk, size_t expected_size)
{
	assert(chunk != NULL);
	assert(chunk->memory != NULL);
	assert(chunk->size == expected_size);
	assert(chunk->used == 0);
	assert(chunk->next == NULL);
}
