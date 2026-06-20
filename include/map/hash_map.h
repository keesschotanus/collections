/**
 * @file hash_map.h
 * @brief Simple Hash map implementation.
 * @details A hash map allows you to store key/value pairs.
 * The key has to be unique and the value can be any type.
 * 
 * Storing and retrieving values is very fast, but the order of the elements is not guaranteed.
 * 
 * Don't forget to free the hash map with hash_map_free() when you're done to avoid memory leaks.
 * 
 * @see hash_map.c for implementation details.
 * @example
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <string.h>
 * #include <assert.h>
 * 
 * #include "map/hash_map.h"
 * 
 * struct person {
 *     int id;
 *     char name[50];
 *     char email[100];
 * };
 * 
 * int main(void)
 * {
 *         hash_map hmap = hash_map_create(10, sizeof(int), sizeof(struct person));
 * 
 *         struct person p1 = {1, "Alice", "alice@example.com"};
 *         struct person p2 = {2, "Bob", "bob@example.com"};
 * 
 *         hash_map_put(hmap, &(int){1}, &p1);
 *         hash_map_put(hmap, &(int){2}, &p2);
 * 
 *         struct person alice = *(struct person *)hash_map_get(hmap, &(int){1});
 *         printf("Alice: id=%d, name=%s, email=%s\n", alice.id, alice.name, alice.email);
 *
 *         hash_map_remove(hmap, &(int){1});
 *         hash_map_free(hmap);
 * }
 * @endcode
 * 
 * In the example above, a hash map is created to hold key/value pairs,
 * where the key is an integer and the value is a struct person.
 * The hash map is created with 10 buckets, and the key and value sizes are specified.
 * The hash_map_put() function is used to insert key/value pairs, and the hash_map_get() function
 * is used to retrieve values based on their keys.
 * The hash_map_remove() function is used to remove a key/value pair from the hash map.
 * Finally, the hash_map_free() function is called to free the memory used by the hash map.
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct hash_map *hash_map_t;

/**
 * @brief Creates a new hash map.
 *
 * @param buckets Number of buckets .
 * @param key_size Size of each key in bytes.
 * @param value_size Size of each value in bytes.
 * @return Pointer to the new hash map, or NULL if allocation failed or sizes are 0.
 */
hash_map_t hash_map_create(size_t buckets, size_t key_size, size_t value_size);

/**
 * @brief Retrieves the value associated with a key in the hash map.
 *
 * @param hmap Pointer to the hash map.
 * @param key Pointer to the key to search for. Must not be NULL.
 * @return Pointer to the value if found, NULL otherwise.
 */
void *hash_map_get(hash_map_t hmap, const void *key);

/**
 * @brief Inserts or updates a key-value pair in the hash map.
 *
 * @param hmap Pointer to the hash map.
 * @param key Pointer to the key to insert/update. Must not be NULL.
 * @param value Pointer to the value to associate with the key. Must not be NULL.
 * @return true if successful, false if allocation failed or hmap/key/value is NULL.
 */
bool hash_map_put(hash_map_t hmap, const void *key, const void *value);

/**
 * @brief Removes a key-value pair from the hash map.
 *
 * @param hmap Pointer to the hash map.
 * @param key Pointer to the key to remove. Must not be NULL.
 * @return true if found and removed, false otherwise.
 */
bool hash_map_remove(hash_map_t hmap, const void *key);

/**
 * @brief Frees the memory used by the hash map.
 *
 * @param hmap Pointer to the hash map to free. If NULL, does nothing.
 */
void hash_map_free(hash_map_t hmap);

#endif