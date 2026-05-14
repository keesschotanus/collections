/**
 * @file hash_map.h
 * @brief Simple Hash map implementation.
 *
 * This header provides an opaque hash map type and functions to manipulate it.
 * 
 * Don't forget to free the hash map with hash_map_free() when you're done to avoid memory leaks.
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct hash_map *hash_map;

/**
 * @brief Creates a new hash map.
 *
 * @param buckets Number of buckets .
 * @param key_size Size of each key in bytes.
 * @param value_size Size of each value in bytes.
 * @return Pointer to the new hash map, or NULL if allocation failed or sizes are 0.
 */
hash_map hash_map_create(size_t buckets, size_t key_size, size_t value_size);

/**
 * @brief Retrieves the value associated with a key in the hash map.
 *
 * @param hmap Pointer to the hash map.
 * @param key Pointer to the key to search for. Must not be NULL.
 * @return Pointer to the value if found, NULL otherwise.
 */
void *hash_map_get(hash_map hmap, const void *key);

/**
 * @brief Inserts or updates a key-value pair in the hash map.
 *
 * @param hmap Pointer to the hash map.
 * @param key Pointer to the key to insert/update. Must not be NULL.
 * @param value Pointer to the value to associate with the key. Must not be NULL.
 * @return true if successful, false if allocation failed or hmap/key/value is NULL.
 */
bool hash_map_put(hash_map hmap, const void *key, const void *value);


#endif