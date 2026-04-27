#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doubly_linked_list.h"

struct doubly_linked_list_node
{
	void *data;
	struct doubly_linked_list_node *prev;
	struct doubly_linked_list_node *next;
};

struct doubly_linked_list
{
	struct doubly_linked_list_node *head; // Dummy head node for circular list
	size_t element_size;
};

dllist dllist_create(size_t element_size)
{
	if (element_size == 0)
		return NULL;

	struct doubly_linked_list *l = malloc(sizeof *l);
	if (l == NULL)
		return NULL;

	l->head = malloc(sizeof(struct doubly_linked_list_node));
	if (l->head == NULL)
	{
		free(l);
		return NULL;
	}
	l->head->data = NULL;
	l->head->next = l->head;
	l->head->prev = l->head;
	l->element_size = element_size;

	return l;
}

bool dllist_insert_before(dllist l, dllnode node, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (node == NULL)
		node = l->head->next;

	struct doubly_linked_list_node *new_node = malloc(sizeof(struct doubly_linked_list_node));
	if (new_node == NULL)
	{
		errno = ENOMEM;
		return false;
	}

	new_node->data = malloc(l->element_size);
	if (new_node->data == NULL)
	{
		free(new_node);
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

	struct doubly_linked_list_node *new_node = malloc(sizeof(struct doubly_linked_list_node));
	if (new_node == NULL)
	{
		errno = ENOMEM;
		return false;
	}

	new_node->data = malloc(l->element_size);
	if (new_node->data == NULL)
	{
		free(new_node);
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
	free(node->data);
	free(node);

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

	struct doubly_linked_list_node *current = l->head->next;
	while (current != l->head)
	{
		// Get the next node before freeing the current one
		struct doubly_linked_list_node *next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	free(l->head);
	free(l);
}