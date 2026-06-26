/**
 * @file doubly_linked_list.c
 * @brief Doubly linked list implementation that can hold elements of any type.
 * @details 
 * The list uses a chunk-based memory allocator to efficiently manage memory for nodes.
 * When nodes are removed, they are added to a free list for reuse, which can improve
 * performance in scenarios with frequent insertions and deletions.
 * 
 * When inserting nodes, if the current chunk doesn't have enough space,
 * a new chunk is allocated with space for at least the initial capacity of nodes.
 * This helps to reduce the number of allocations.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the list
 * from users of the API.
 * 
 * My design goal was to keep things simple, but after my initial implementation,
 * I asked AI about the interview with Linus Torvalds and good taste in programming,
 * as I new it was related to handling a linked list.
 * AI suggested to use a circular list with a dummy head node since it would simplify edge cases.
 * That made the code much cleaner and easier to maintain, so I went with that design.
 * 
 * The code worked well and passed all tests, but I was worried about performance
 * when inserting a large number of elements.
 * Particularly since each node requires a separate memory allocation, which can be expensive.
 * To address this, I asked AI to implement a simple chunk allocator that allocates memory
 * for multiple nodes at once.
 * 
 * I measured the performance of inserting 100 million integers into the list.
 * Before implementing the chunk allocator this required 3.1s and after implementing it,
 * the time dropped to 1.0s.
 * This on a 13th Gen Intel(R) Core(TM) i7-13700.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list/doubly_linked_list.h"
#include "util/chunk.h"

#define INITIAL_CAPACITY 32

struct doubly_linked_list_node
{
	dllnode_t prev;
	dllnode_t next;
	char data[]; // flexible array member (must be last)
};

struct doubly_linked_list
{
	dllnode_t head; // Dummy head node for circular list
	size_t number_of_elements;
	size_t element_size;
	chunk_t chunks;
	dllnode_t free_list;
	size_t initial_capacity;
};

static dllnode_t allocate_node(dllist_t l);
static void free_node(dllist_t l, dllnode_t node);

dllist_t dllist_create(size_t initial_capacity, size_t element_size)
{
	if (element_size == 0)
		return NULL;

	dllist_t l = malloc(sizeof *l);
	if (l == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}
	
	l->number_of_elements = 0;
	l->element_size = element_size;
	l->chunks = NULL;
	l->free_list = NULL;
	l->initial_capacity = initial_capacity > 0 ? initial_capacity : INITIAL_CAPACITY;

	l->head = allocate_node(l);
	if (l->head == NULL)
	{
		free(l);
		errno = ENOMEM;
		return NULL;
	}
	l->head->next = l->head;
	l->head->prev = l->head;

	return l;
}

bool dllist_insert_before(dllist_t l, dllnode_t node, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (node == NULL)
		node = l->head->next;

	dllnode_t new_node = allocate_node(l);
	if (new_node == NULL)
	{
		errno = ENOMEM;
		return false;
	}

	memcpy(new_node->data, element, l->element_size);
	new_node->prev = node->prev;
	new_node->next = node;
	node->prev->next = new_node;
	node->prev = new_node;

	++l->number_of_elements;
	return true;
}

bool dllist_insert_after(dllist_t l, dllnode_t node, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (node == NULL)
		node = l->head->prev;

	dllnode_t new_node = allocate_node(l);
	if (new_node == NULL)
	{
		errno = ENOMEM;
		return false;
	}

	memcpy(new_node->data, element, l->element_size);
	new_node->prev = node;
	new_node->next = node->next;
	node->next->prev = new_node;
	node->next = new_node;

	++l->number_of_elements;
	return true;
}

bool dllist_push(dllist_t l, const void *element)
{
	return dllist_insert_after(l, NULL, element);
}

void *dllist_pop(dllist_t l)
{
	if (l == NULL || l->head->prev == l->head)
		return NULL;

	dllnode_t last_node = l->head->prev;
	dllist_remove(l, last_node);
	return last_node->data;
}

void *dllist_peek(dllist_t l)
{
	if (l == NULL || l->head->prev == l->head)
		return NULL;

	dllnode_t last_node = l->head->prev;
	return last_node->data;
}

dllnode_t dllist_first(dllist_t l)
{
	if (l == NULL || l->head->next == l->head)
		return NULL;
	return l->head->next;
}

dllnode_t dllist_last(dllist_t l)
{
	if (l == NULL || l->head->prev == l->head)
		return NULL;
	return l->head->prev;
}

dllnode_t dllist_next(dllist_t l, dllnode_t node)
{
	if (l == NULL || node == NULL || node->next == l->head)
		return NULL;
	return node->next;
}

dllnode_t dllist_prev(dllist_t l, dllnode_t node)
{
	if (l == NULL || node == NULL || node->prev == l->head)
		return NULL;
	return node->prev;
}

bool dllist_remove(dllist_t l, dllnode_t node)
{
	if (l == NULL || node == NULL)
		return false;

	node->prev->next = node->next;
	node->next->prev = node->prev;
	free_node(l, node);

	--l->number_of_elements;
	return true;
}

dllnode_t dllist_find(dllist_t l, const void *element, int (*cmp)(const void *, const void *))
{
	if (l == NULL || element == NULL || cmp == NULL)
		return NULL;

	dllnode_t current = l->head->next;
	while (current != l->head)
	{
		if (cmp(current->data, element) == 0)
		{
			return current;
		}
		current = current->next;
	}

	return NULL;
}

void dllist_visit(dllist_t l, void (*visit)(const void *))
{
	if (l == NULL || visit == NULL)
		return;

	dllnode_t current = l->head->next;
	while (current != l->head)
	{
		visit(current->data);
		current = current->next;
	}
}

const void *dllist_node_data(dllnode_t node)
{
	if (node == NULL)
		return NULL;
	return node->data;
}

void dllist_free(dllist_t l)
{
	if (l == NULL)
		return;
	chunk_free(l->chunks);
	free(l);
}

size_t dllist_size(dllist_t l)
{
	return l ? l->number_of_elements : 0;
}


static void free_node(dllist_t l, dllnode_t node)
{
	node->next = l->free_list;
	l->free_list = node;
}

static dllnode_t allocate_node(dllist_t l)
{
	if (l->free_list != NULL)
	{
		// reuse node from free list
		dllnode_t node = l->free_list;
		l->free_list = node->next;
		return node;
	}
	size_t node_size = sizeof(struct doubly_linked_list_node) + l->element_size;
	if (l->chunks == NULL || l->chunks->used + node_size > l->chunks->size)
	{
		// allocate new chunk
		chunk_t new_chunk = chunk_allocate(l->initial_capacity, node_size);
		if (new_chunk == NULL)
			return NULL;

		new_chunk->next = l->chunks;
		l->chunks = new_chunk;
	}
	dllnode_t node = (dllnode_t)(l->chunks->memory + l->chunks->used);
	l->chunks->used += node_size;

	return node;
}
