/*
 * list.h
 *
 * A simple dynamic array implementation in C.
 *
 * This header defines an opaque list type and functions to create, manipulate,
 * and free the list. The list can hold elements of any type, specified by
 * the element size at creation.
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @file list.h
 * @brief Dynamic array implementation in C.
 *
 * This header provides an opaque dynamic list type and functions to manipulate it.
 */

typedef struct list *list;

/**
 * @brief Creates a new dynamic list.
 *
 * Allocates memory for a new list with the specified initial capacity and element size.
 *
 * @param capacity Initial capacity of the list (number of elements). If 0, uses a default.
 * @param element_size Size of each element in bytes. Must be greater than 0.
 * @return Pointer to the new list, or NULL if allocation failed or element_size is 0.
 */
list list_create(size_t capacity, size_t element_size);

/**
 * @brief Appends an element to the end of the list.
 *
 * If the list is full, it will automatically resize to accommodate the new element.
 *
 * @param l Pointer to the list.
 * @param element Pointer to the element to append. Must not be NULL.
 */
void list_append(list l, const void *element);

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