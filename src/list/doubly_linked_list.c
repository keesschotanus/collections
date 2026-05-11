#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doubly_linked_list.h"

struct chunk
{
	struct chunk *next;
	char *memory;
	size_t size;
	size_t used;
};

struct doubly_linked_list_node
{
	struct doubly_linked_list_node *prev;
	struct doubly_linked_list_node *next;
	char data[]; // flexible array member (must be last)
};

struct doubly_linked_list
{
	struct doubly_linked_list_node *head; // Dummy head node for circular list
	size_t element_size;
	struct chunk *chunks;
	struct doubly_linked_list_node *free_list;
};

static struct doubly_linked_list_node *allocate_node(struct doubly_linked_list *l);
static void free_node(struct doubly_linked_list *l, struct doubly_linked_list_node *node);

dllist dllist_create(size_t element_size)
{
	if (element_size == 0)
		return NULL;

	struct doubly_linked_list *l = malloc(sizeof *l);
	if (l == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}
	
	l->element_size = element_size;
	l->chunks = NULL;
	l->free_list = NULL;

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

bool dllist_insert_before(dllist l, dllnode node, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (node == NULL)
		node = l->head->next;

	struct doubly_linked_list_node *new_node = allocate_node(l);
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

	return true;
}

bool dllist_insert_after(dllist l, dllnode node, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (node == NULL)
		node = l->head->prev;

	struct doubly_linked_list_node *new_node = allocate_node(l);
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

	return true;
}

dllnode dllist_next(dllist l, dllnode node)
{
	if (l == NULL || node == NULL || node->next == l->head)
		return NULL;
	return node->next;
}

dllnode dllist_prev(dllist l, dllnode node)
{
	if (l == NULL || node == NULL || node->prev == l->head)
		return NULL;
	return node->prev;
}

bool dllist_remove(dllist l, dllnode node)
{
	if (l == NULL || node == NULL)
		return false;

	node->prev->next = node->next;
	node->next->prev = node->prev;
	free_node(l, node);

	return true;
}

dllnode dllist_find(dllist l, const void *element, int (*cmp)(const void *, const void *))
{
	if (l == NULL || element == NULL || cmp == NULL)
		return NULL;

	struct doubly_linked_list_node *current = l->head->next;
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

const void *dllist_node_data(dllnode node)
{
	if (node == NULL)
		return NULL;
	return node->data;
}

dllnode dllist_first(dllist l)
{
	if (l == NULL || l->head->next == l->head)
		return NULL;
	return l->head->next;
}

dllnode dllist_last(dllist l)
{
	if (l == NULL || l->head->prev == l->head)
		return NULL;
	return l->head->prev;
}

void dllist_free(dllist l)
{
	if (l == NULL)
		return;

	// free all chunks
	struct chunk *chunk = l->chunks;
	while (chunk != NULL)
	{
		struct chunk *next = chunk->next;
		free(chunk->memory);
		free(chunk);
		chunk = next;
	}
	free(l);
}

static void free_node(struct doubly_linked_list *l, struct doubly_linked_list_node *node)
{
	node->next = l->free_list;
	l->free_list = node;
}

static struct doubly_linked_list_node *allocate_node(struct doubly_linked_list *l)
{
	if (l->free_list != NULL)
	{
		// reuse node from free list
		struct doubly_linked_list_node *node = l->free_list;
		l->free_list = node->next;
		return node;
	}
	size_t node_size = sizeof(struct doubly_linked_list_node) + l->element_size;
	if (l->chunks == NULL || l->chunks->used + node_size > l->chunks->size)
	{
		// allocate new chunk
		struct chunk *new_chunk = malloc(sizeof(struct chunk));
		if (new_chunk == NULL)
			return NULL;

		// Allocate at least room for 32 nodes or 1MB, whichever is larger
		size_t elements_to_allocate = 32 * node_size >= 1024 * 1024 ? 32 : (1024 * 1024 / node_size) + 1;
		size_t alloc_size = elements_to_allocate * node_size;
			
		new_chunk->size = alloc_size;
		new_chunk->memory = malloc(alloc_size);
		if (new_chunk->memory == NULL)
		{
			free(new_chunk);
			return NULL;
		}
		new_chunk->used = 0;
		new_chunk->next = l->chunks;
		l->chunks = new_chunk;
	}
	struct doubly_linked_list_node *node = (struct doubly_linked_list_node *)(l->chunks->memory + l->chunks->used);
	l->chunks->used += node_size;
	return node;
}
