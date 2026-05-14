/**
 * @file hash_map.c
 * 
 * This file implements a simple hash map data structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "doubly_linked_list.h"
#include "hash_map.h"

#define INITIAL_BUCKET_CAPACITY 10

struct hash_map
{
	void *bucket_list;
	size_t bucket_list_size;
	size_t number_of_elements;
	size_t key_size;
	size_t value_size;
};

struct bucket_list {
	dllist list;
};

static size_t hash(const void *key, size_t key_size, size_t num_buckets);

hash_map hash_map_create(size_t buckets, size_t key_size, size_t value_size) {
	if (buckets == 0 || key_size == 0 || value_size == 0)
		return NULL;

	list bucket_list = list_create(buckets, sizeof(dllist));
	if (bucket_list == NULL)
		return NULL;

	// Currently the only way to populate the buckets is by using list_append().
	// This means we need to append NULL for each bucket to initialize them.
	for (size_t i = 0; i < buckets; i++) {
		dllist null_bucket = NULL;
		if (!list_append(bucket_list, &null_bucket)) {
			list_free(bucket_list);
			return NULL;
		}
	}

	struct hash_map *hmap = malloc(sizeof(struct hash_map));
	if (hmap == NULL)
	{
		list_free(bucket_list);
		return NULL;
	}

	hmap->bucket_list = bucket_list;
	hmap->bucket_list_size = buckets;
	hmap->number_of_elements = 0;
	hmap->key_size = key_size;
	hmap->value_size = value_size;
	return hmap;
}

bool hash_map_put(hash_map hmap, const void *key, const void *value) {
	if (hmap == NULL || key == NULL || value == NULL)
		return false;

	// Get the bucket index using the hash function
	size_t bucket_index = hash(key, hmap->key_size, hmap->bucket_list_size);
	dllist bucket = *(dllist *)list_get(hmap->bucket_list, bucket_index);
	if (bucket == NULL)
	{
		// Create a new bucket
		bucket = dllist_create(INITIAL_BUCKET_CAPACITY, hmap->key_size + hmap->value_size);
		if (bucket == NULL)
			return false;

		list_set(hmap->bucket_list, bucket_index, &bucket);
	}

	struct {
		char key[hmap->key_size];
		char value[hmap->value_size];
	} entry;

	memcpy(entry.key, key, hmap->key_size);
	memcpy(entry.value, value, hmap->value_size);

	return dllist_push(bucket, &entry);
}

void *hash_map_get(hash_map hmap, const void *key) {
	if (hmap == NULL || key == NULL)
		return NULL;

	// Get the bucket index using the hash function
	size_t bucket_index = hash(key, hmap->key_size, hmap->bucket_list_size);
	dllist bucket = *(dllist *)list_get(hmap->bucket_list, bucket_index);
	if (bucket == NULL)
		return NULL;

	// Iterate through the bucket to find the matching key
	dllnode node = dllist_first(bucket);
	while (node != NULL) {
		const void *entry = dllist_node_data(node);
		// Compare only the key part of the entry
		if (memcmp(entry, key, hmap->key_size) == 0) {
			return (char *)entry + hmap->key_size;
		}
		node = dllist_next(bucket, node);
	}

	return NULL;
}

void hash_map_free(hash_map hmap) {
	if (hmap == NULL)
		return;

	// Free each bucket list
	size_t buckets = list_size(hmap->bucket_list);
	for (size_t i = 0; i < buckets; i++)
	{
		void *bucket = list_get(hmap->bucket_list, i);
		if (bucket != NULL)
			dllist_free(bucket);
	}

	// Free the bucket list and the hash map itself
	list_free(hmap->bucket_list);
	free(hmap);
}

static size_t hash(const void *key, size_t key_size, size_t num_buckets) {	
	size_t hash_value = 5381;
	const unsigned char *ptr = (const unsigned char *)key;

	for (size_t i = 0; i < key_size; ++i)
	{
		hash_value = ((hash_value << 5) + hash_value) + *(ptr + i);
	}

	return hash_value % num_buckets;
}

