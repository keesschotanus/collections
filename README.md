# Collections

Basic implementation of some collections in the C programming language.
We all need lists, maps and trees and it would be wise to use a standard library for this,
but why use a standard library in minutes when you can experience the agony of writing some of these yourself?

## Lists

### Simple dynamic list

Example:

```C
#include "list.h"

int main(void)
{
	// Create a list of integers
	list int_list = list_create(10, sizeof(int));

	for (size_t i = 0; i < 20; i++)
		list_append(int_list, &i);

	// Set a value at index 3
	int value = 42;
	list_set(int_list, 3, &value);

	// Get a value from index 7
	value = *(int *)list_get(int_list, 7);

	list_free(int_list);

    return 0;
}
```

In the example above, a list of integers is created, with an initial capacity of 10 elements.
Since 20 elements are added to the list, the list is automatically grown when the maximum capacity is reached.
The list is made to hold any type, not only integers.

You can only add elements to the end of the list.
You can't insert elements anywhere else and you can't remove items from the list.
For this you need a linked list.

### Doubly Linked List

Usage:
```C
#include "doubly_linked_list.h"

static int compare_ints(const void *a, const void *b)
{
        const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;

	return (*int_a > *int_b) - (*int_a < *int_b);
}

int main(void)
{
	dllist l = dllist_create(sizeof(int));
	// Add at beginning
	dllist_insert_before(l, NULL, &(int){1});

        // Find the node with a value of 1
	dllnode node = dllist_find(l, &(int){1}, compare_ints);
	// Insert a node after the found node
	dllist_insert_after(l, node, &(int){2});
	// Remove the last node from the list
	dllist_remove(l, dllist_last(l));

	return 0;
}
```

In the example above, a doubly linked list is created to hold elements of type int.
Other types work similarly.
The first integer is stored at the head of the list.
You can also insert elements to the tail and before or after a specific node
that you locate using the dllist_find function.

Note that you need a function to compare two elements of the same type.
In the example compare_ints is used.