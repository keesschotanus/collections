/*
 * test_hash.c
 *
 * Unit tests for the hash function implementation defined in hash.c.
 * 
 * Since my hash function uses a well known algorithm, only some
 * common and edge cases are tested.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "util/hash.h"

static void test_hash_basic(void);
static void test_hash_edges(void);

int test_hash(void)
{
	printf("Test hash function...");

	test_hash_basic();
	test_hash_edges();

	puts(" ✅");
	return 0;
}

static void test_hash_basic(void)
{
	const char* hello = "Hello World!";
	size_t hash_value1 = hash(hello, strlen(hello));
	size_t hash_value2 = hash(hello, strlen(hello));

	// Same value should result in same hash
	assert(hash_value1 == hash_value2);

	// Different value should result in different hash
	hash_value2 = hash(hello, strlen(hello) -1);
	assert(hash_value1 != hash_value2);
	
}

static void test_hash_edges(void)
{
	// An empty string should get an actual hashed value
	assert(hash("", 0) != 0);
}

