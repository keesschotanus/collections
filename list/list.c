#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 30 	// As a percentage of current capacity

struct list
{
	void *data;
	size_t number_of_elements;
	size_t capacity;
	size_t element_size;
};

list list_create(size_t capacity, size_t element_size)
{
	if (element_size == 0)
		return NULL;

	// Prevent zero byte allocation by using a default initial capacity	
	capacity = capacity > 0 ? capacity : INITIAL_CAPACITY;

	struct list *l = malloc(sizeof *l);
	if (l == NULL)
		return NULL;

	l->data = malloc(capacity * element_size);
	if (l->data == NULL)
	{
		free(l);
		return NULL;
	}

	l->number_of_elements = 0;
	l->capacity = capacity;
	l->element_size = element_size;
	return l;
}

void list_append(list l, const void *element)
{
	if (l == NULL || element == NULL)
		return;

	if (l->number_of_elements == l->capacity)
	{
		size_t additional_capacity = l->capacity * GROWTH_FACTOR / 100;
		additional_capacity = additional_capacity >= INITIAL_CAPACITY
			? additional_capacity : INITIAL_CAPACITY;

		void *tmp = realloc(l->data, (l->capacity + additional_capacity) * l->element_size);
		if (tmp == NULL)
		{
			perror("Memory resize failed for list");
			return;
		}

		l->data = tmp;
		l->capacity += additional_capacity;
	}

	memcpy((char *)l->data + l->number_of_elements * l->element_size, element, l->element_size);
	l->number_of_elements++;
}

void *list_get(list l, size_t idx)
{
	if (l == NULL || idx >= l->number_of_elements)
		return NULL;

	return (char *)l->data + idx * l->element_size;
}

bool list_set(list l, size_t idx, const void *element)
{
	if (l == NULL || element == NULL || idx >= l->number_of_elements)
		return false;

	memcpy((char *)l->data + idx * l->element_size, element, l->element_size);
	return true;
}

void list_free(list l)
{
	if (l == NULL)
		return;

	free(l->data);
	free(l);
}

size_t list_size(list l)
{
	return l ? l->number_of_elements : 0;
}
