/**
 * @file list.h
 * @brief Dynamic list implementation (dynamic array).
 * @details A dynamic list is a data structure that can grow dynamically.
 * The list can hold elements of any type, specified by the element size at creation.
 * When elements are added, the list automatically resizes.
 * Elements can only be appended to the end of the list.
 * If you need more complex operations like inserting at specific positions,
 * consider using the doubly_linked_list.h instead.
 * 
 * Don't forget to free the list with list_free() when you're done to avoid memory leaks.
 * @example
 * @code
 * #include "list.h"
 * 
 * int main(void)
 * {
 *         // Create a list of integers
 *         list int_list = list_create(10, sizeof(int));
 *
 *         for (size_t i = 0; i < 20; i++)
 *                 list_append(int_list, &i);
 *
 *         // Set a value at index 3
 *         int value = 42;
 *         list_set(int_list, 3, &value);
 *
 *         // Get a value from index 7
 *         value = *(int *)list_get(int_list, 7);
 *
 *         list_free(int_list);
 *
 *         return 0;
 * }
 * @endcode
 * In the example above, a list of integers is created, with an initial capacity of 10 elements.
 * Since 20 elements are added to the list, the list is automatically grown when the maximum capacity is reached.
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>


typedef struct list *list;

/**
 * @brief Creates a new dynamic list.
 *
 * Allocates memory for a new list with the specified initial capacity and element size.
 *
 * @param initial_capacity Initial capacity of the list (number of elements).
 * @param element_size Size of each element in bytes.
 * @return Pointer to the new list, or NULL if allocation failed or element_size is 0.
 */
list list_create(size_t initial_capacity, size_t element_size);

/**
 * @brief Appends an element to the end of the list.
 *
 * If the list is full, it will automatically resize to accommodate the new element.
 *
 * @param l Pointer to the list.
 * @param element Pointer to the element to append.
 * @return true if successful, false if allocation failed or list is NULL.
 */
bool list_append(list l, const void *element);

/**
 * @brief Retrieves an element from the list at the specified index.
 *
 * @param l Pointer to the list.
 * @param idx Index of the element to retrieve (0-based).
 * @return Pointer to the element, or NULL if index is out of bounds or list is NULL.
 */
void *list_get(list l, size_t idx);

/**
 * @brief Sets the value of an element at the specified index.
 *
 * @param l Pointer to the list.
 * @param idx Index of the element to set (0-based).
 * @param element Pointer to the new element value. Must not be NULL.
 * @return true if successful, false if index is out of bounds or list/element is NULL.
 */
bool list_set(list l, size_t idx, const void *element);

/**
 * @brief Visits each element in the list using the provided function.
 *
 * @param l Pointer to the list.
 * @param visit Pointer to the visit function, called for each element.
 */
void list_visit(list l, void (*visit)(const void *));

/**
 * @brief Frees the memory allocated for the list.
 *
 * After calling this, the list pointer becomes invalid.
 *
 * @param l Pointer to the list to free.
 */
void list_free(list l);

/**
 * @brief Returns the number of elements currently in the list.
 *
 * @param l Pointer to the list.
 * @return Number of elements in the list, or 0 if list is NULL.
 */
size_t list_size(list l);

/**
 * @brief Sorts the elements in the list using the provided comparison function.
 *
 * @param l Pointer to the list.
 * @param cmp Pointer to the comparison function. Must not be NULL.
 *  The function should return a negative value if the first argument is less than the second,
 *  zero if they are equal, and a positive value if the first argument is greater than the second.
 */
void sort_list(list l, int (*cmp)(const void *, const void *));

#endif