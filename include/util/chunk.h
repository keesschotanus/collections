/**
 * @file chunk.h        
 * @brief Allocator to allocate memory in chunks.
 * @details This file defines a simple chunk allocator that allocates memory in chunks.
 * It is used to allocate memory for nodes of a constant size
 * to avoid frequent allocations and deallocations.
 * @see chunk.c for implementation details.
 */

#ifndef CHUNK_H
#define CHUNK_H

#include <stddef.h>

struct chunk
{
	struct chunk *next;
	char *memory;
	size_t size;
	size_t used;
};

typedef struct chunk chunk;
typedef struct chunk *chunk_t;

chunk_t chunk_allocate(size_t node_count, size_t node_size);
void chunk_free(chunk_t chunk);

#endif