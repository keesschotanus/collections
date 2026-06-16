/**
 * @file hash.h
 * @brief Hash function.
 */

#ifndef HASH_H
#define HASH_H

#include <stddef.h>

/**
 * @brief Computes a hash value for the given key.
 * 
 * @param key The key to hash.
 * @param key_size The size of the key in bytes.
 * @return The computed hash value.
 */
size_t hash(const void *key, size_t key_size);

#endif
