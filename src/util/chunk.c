/**
 * @file chunk_allocator.c
 * @brief Memory is allocated in chunks to prevent fragmentation and improve performance.
 * @details This file implements a simple chunk allocator that allocates memory in chunks.
 * It is used to allocate memory for nodes of a constant size.
 * 
 * It is the responsibility of the caller to free the memory allocated by this allocator,
 * by calling chunk_free() with the head chunk.
 */

#include <stddef.h>
#include <stdlib.h>

#include "util/chunk.h"

chunk_t chunk_allocate(size_t node_count, size_t node_size)
{       
	chunk_t new_chunk = malloc(sizeof *new_chunk);
	if (new_chunk == NULL)
		return NULL;

	size_t alloc_size = node_count * node_size;
	new_chunk->size = alloc_size;
	new_chunk->memory = malloc(alloc_size);
	if (new_chunk->memory == NULL)
	{
		free(new_chunk);
		return NULL;
	}

		new_chunk->used = 0;
	    return new_chunk;
}

	void chunk_free(chunk_t chunk)
	{
		if (chunk == NULL)
			return;

		chunk_t current = chunk->next;
		while (current != NULL)
		{
			chunk_t next = current->next;
			free(current->memory);
			free(current);
			current = next;
		}

	    /* Free memory allocated for the head chunk */
	    free(chunk->memory);
	    free(chunk);
	}