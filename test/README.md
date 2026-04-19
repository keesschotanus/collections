# Collections

Basic implementation of some collections in the C programming language.
We all need lists, maps and trees and it would be wise to use a standard library for this,
but why use a standard library in minutes when you can experience the agony of writing some of these yourself?

## Lists

### Simple dynamic list

Usage:

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

To create a list of doubles use: ```list dbl_list = list_create(10, sizeof(double));```
You should now be able to store and retrieve double values from this list.