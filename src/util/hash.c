/**
 * @file hash.c
 * @brief Hash function implementation using the djb2 algorithm.
 * @details This hash function is widely used and provides good distribution.
 * It was created by Dan Bernstein and is known for its simplicity and performance.
 */

#include <stddef.h>

#include "util/hash.h"

size_t hash(const void *key, size_t key_size) {	
	size_t hash_value = 5381;
	const unsigned char *ptr = (const unsigned char *)key;

	for (size_t i = 0; i < key_size; ++i)
	{
		hash_value = ((hash_value << 5) + hash_value) + *(ptr + i);
	}

	return hash_value;
}
