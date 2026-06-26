/**
 * @file hash_map.c
 * @brief Simple Hash map implementation.
 * @details A hash map allows you to store key/value pairs.
 * The key has to be unique and the value can be any type.

 * I used my dynamic list to store a list of buckets.
 * Each bucket is a doubly linked list that holds key-value pairs.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the list
 * from users of the API.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list/list.h"
#include "list/doubly_linked_list.h"
#include "map/hash_map.h"
#include "util/hash.h"

#define INITIAL_BUCKET_CAPACITY 10

struct hash_map
{
	list_t bucket_list;
	size_t bucket_list_size;
	size_t number_of_elements;
	size_t key_size;
	size_t value_size;
};

static dllist_t get_bucket(hash_map_t hmap, const void *key);
static dllnode_t get_node_from_bucket(hash_map_t hmap, dllist_t bucket, const void *key);

hash_map_t hash_map_create(size_t buckets, size_t key_size, size_t value_size) {
	if (buckets == 0 || key_size == 0 || value_size == 0)
		return NULL;

	list_t bucket_list = list_create(buckets, sizeof(dllist_t));
	if (bucket_list == NULL)
		return NULL;
	dllist_t null_bucket = NULL;        
	for (size_t i = 0; i < buckets; i++) {
		
		list_append(bucket_list, &null_bucket);
	}

		hash_map_t hmap = malloc(sizeof *hmap);
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

bool hash_map_put(hash_map_t hmap, const void *key, const void *value) {
	if (hmap == NULL || key == NULL || value == NULL)
		return false;

	size_t bucket_index = hash(key, hmap->key_size) % hmap->bucket_list_size;
	dllist_t bucket = *(dllist_t *)list_get(hmap->bucket_list, bucket_index);
	if (bucket == NULL)
	{
		// Create a new bucket
		bucket = dllist_create(INITIAL_BUCKET_CAPACITY, hmap->key_size + hmap->value_size);
		if (bucket == NULL)
			return false;

		list_set(hmap->bucket_list, bucket_index, &bucket);
	} else {
		// Check if the key is already present
		if (hash_map_get(hmap, key) != NULL)
			return false;
	}

	struct {
		char key[hmap->key_size];
		char value[hmap->value_size];
	} entry;

	memcpy(entry.key, key, hmap->key_size);
	memcpy(entry.value, value, hmap->value_size);

	++hmap->number_of_elements;
	return dllist_push(bucket, &entry);
}

// TODO hash_map_get() returns a pointer to the value inside the bucket node;
// User code can hold pointers to internal storage.
void *hash_map_get(hash_map_t hmap, const void *key) {
	if (hmap == NULL || key == NULL)
		return NULL;

	dllist_t bucket = get_bucket(hmap, key);
	if (bucket == NULL)
		return NULL;

	dllnode_t node = get_node_from_bucket(hmap, bucket, key);
	if (node == NULL)
		return NULL;

	return (char *)dllist_node_data(node) + hmap->key_size;
}

bool hash_map_remove(hash_map_t hmap, const void *key) {
	if (hmap == NULL || key == NULL)
		return false;

	dllist_t bucket = get_bucket(hmap, key);
	if (bucket == NULL)
		return false;

	dllnode_t node = get_node_from_bucket(hmap, bucket, key);
	if (node == NULL)
		return false;

	--hmap->number_of_elements;
	return dllist_remove(bucket, node);
}

size_t hash_map_size(hash_map_t hmap) {
	return hmap ? hmap->number_of_elements : 0;
}

void hash_map_free(hash_map_t hmap) {
	if (hmap == NULL)
		return;

	// Free each bucket
	size_t buckets = list_size(hmap->bucket_list);
	for (size_t i = 0; i < buckets; i++)
	{
		dllist_t bucket = *(dllist_t *)list_get(hmap->bucket_list, i);
		dllist_free(bucket);
	}

	// Free the bucket list and the hash map itself
	list_free(hmap->bucket_list);
	free(hmap);
}

static dllist_t get_bucket(hash_map_t hmap, const void *key) {
	if (hmap == NULL || key == NULL)
		return NULL;

	size_t bucket_index = hash(key, hmap->key_size) % hmap->bucket_list_size;
	return *(dllist_t *)list_get(hmap->bucket_list, bucket_index);
}

static dllnode_t get_node_from_bucket(hash_map_t hmap, dllist_t bucket, const void *key) {
	dllnode_t node = dllist_first(bucket);
	while (node != NULL) {
		const void *entry = dllist_node_data(node);
		// Compare only the key part of the entry
		if (memcmp(entry, key, hmap->key_size) == 0) {
			return node;
		}
		node = dllist_next(bucket, node);
	}

	return NULL;

}	