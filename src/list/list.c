/**
 * @file list.c
 * 
 * This file implements a dynamic list (dynamic array) that can hold elements of any type.
 * The list automatically resizes to twice its current capacity when elements are added.
 * 
 * The implementation uses an opaque pointer to hide the internal structure of the list
 * from users of the API.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list/list.h"

#define INITIAL_CAPACITY 32

struct list
{
	void *data;
	size_t number_of_elements;
	size_t capacity;
	size_t element_size;
};

 list_t list_create(size_t initial_capacity, size_t element_size)
{
	if (element_size == 0)
		return NULL;

	// Prevent zero byte allocation by using a default initial capacity	
	initial_capacity = initial_capacity > 0 ? initial_capacity : INITIAL_CAPACITY;

	list_t l = malloc(sizeof *l);
	if (l == NULL)
		return NULL;

	l->data = malloc(initial_capacity * element_size);
	if (l->data == NULL)
	{
		free(l);
		return NULL;
	}

	l->number_of_elements = 0;
	l->capacity = initial_capacity;
	l->element_size = element_size;
	return l;
}

bool list_append(list_t l, const void *element)
{
	if (l == NULL || element == NULL)
		return false;

	if (l->number_of_elements == l->capacity)
	{
		size_t additional_capacity = l->number_of_elements;
		void *tmp = realloc(l->data, (l->capacity + additional_capacity) * l->element_size);
		if (tmp == NULL)
		{
			errno = ENOMEM;
			return false;
		}

		l->data = tmp;
		l->capacity += additional_capacity;
	}

	memcpy((char *)l->data + l->number_of_elements * l->element_size, element, l->element_size);
	l->number_of_elements++;

	return true;
}

void *list_get(list_t l, size_t idx)
{
	if (l == NULL || idx >= l->number_of_elements)
		return NULL;

	return (char *)l->data + idx * l->element_size;
}

bool list_set(list_t l, size_t idx, const void *element)
{
	if (l == NULL || element == NULL || idx >= l->number_of_elements)
		return false;

	memcpy((char *)l->data + idx * l->element_size, element, l->element_size);
	return true;
}

void list_visit(list_t l, void (*visit)(const void *))
{
	if (l == NULL || visit == NULL)
		return;

	for (size_t i = 0; i < l->number_of_elements; i++)
	{
		visit((char *)l->data + i * l->element_size);
	}
}

void list_free(list_t l)
{
	if (l == NULL)
		return;

	free(l->data);
	free(l);
}

size_t list_size(list_t l)
{
	return l ? l->number_of_elements : 0;
}

void sort_list(list_t l, int (*cmp)(const void *, const void *))
{
	if (l == NULL || cmp == NULL)
		return;

	qsort(l->data, l->number_of_elements, l->element_size, cmp);
}