/**
 * @file doubly_linked_list.h
 * @brief Doubly linked list implementation.
 *
 * This header provides an opaque dynamic doubly linked list type and functions to manipulate it.
 * The list can hold elements of any type, specified by the element size at creation.
 * The list itself is not exposed to the user, and all interactions are done through the provided functions.
 * 
 * Don't forget to free the list with dllist_free() when you're done to avoid memory leaks.
 * 
 * My design goal was to keep things simple, but after my initial implementation, I asked AI about the interview with
 * Linus Torvalds and good taste in programming as I new it was related to handling a linked list.
 * AI suggested to use a circular list with a dummy head node since it would simplify edge cases.
 * That made the code much cleaner and easier to maintain, so I went with that design.
 * 
 * The code worked well and passed all tests, but I was worried about performance when inserting a large number of elements.
 * Particularly since each node requires a separate allocation, which can be expensive.
 * To address this, I asked AI to implement a simple chunk allocator that allocates memory for multiple nodes at once.
 * 
 * I measured the performance of inserting 100 million integers into the list.
 * Before implementing the chunk allocator this required 3.1s and after implementing it, the time dropped to 1.0s.
 * This on a 13th Gen Intel(R) Core(TM) i7-13700.
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>


typedef struct doubly_linked_list *dllist;

typedef struct doubly_linked_list_node *dllnode;

/**
 * @brief Creates a new doubly linked list.
 *
 * @param initial_capacity Initial capacity of the list (number of nodes).
 * @param element_size Size of each element in bytes.
 * @return Pointer to the new list, or NULL if allocation failed or element_size is 0.
 */
dllist dllist_create(size_t initial_capacity, size_t element_size);

/**
 * @brief Inserts an element before the specified node.
 *
 * @param l Pointer to the list.
 * @param node The node before which to insert.
 *  When node is NULL, the element is added at the beginning of the list.
 * @param element Pointer to the element to insert.
 * @return true if successful, false if allocation failed or list is NULL.
 */
bool dllist_insert_before(dllist l, dllnode node, const void *element);

/**
 * @brief Inserts an element after the specified node.
 *
 * @param l Pointer to the list.
 * @param node The node after which to insert.
 *  When node is NULL, the element is added at the end of the list.
 * @param element Pointer to the element to insert.
 * @return true if successful, false if allocation failed or list is NULL.
 */
bool dllist_insert_after(dllist l, dllnode node, const void *element);

/**
 * @brief Gets the first node in the list.
 *
 * @param l Pointer to the list.
 * @return Pointer to the first node, or NULL if the list is empty.
 */
dllnode dllist_first(dllist l);

/**
 * @brief Gets the last node in the list.
 *
 * @param l Pointer to the list.
 * @return Pointer to the last node, or NULL if the list is empty.
 */
dllnode dllist_last(dllist l);

/**
 * @brief Gets the node after the supplied node.
 *
 * @param l Pointer to the list.
 * @param node The current node.
 * @return Pointer to the next node, or NULL if node is NULL or it's the last node.
 */
dllnode dllist_next(dllist l, dllnode node);

/**
 * @brief Gets the node before the supplied node.
 *
 * @param l Pointer to the list.
 * @param node The current node.
 * @return Pointer to the previous node, or NULL if node is NULL or it's the first node.
 */
dllnode dllist_prev(dllist l, dllnode node);

/**
 * @brief Removes a node from the list.
 * 
 * @param l Pointer to the list.
 * @param node The node to remove.
 * @return true if successful, false if list or node is NULL.
 */
bool dllist_remove(dllist l, dllnode node);

/**
 * @brief Finds the first element in the list using a comparison function.
 *
 * @param l Pointer to the list.
 * @param element Pointer to the element to find.
 * @return Pointer to the found node, or NULL if not found.
 */
dllnode dllist_find(dllist l, const void *element, int (*cmp)(const void *, const void *));

/**
 * @brief Visits each element in the list using the provided function.
 *
 * @param l Pointer to the list.
 * @param visit Pointer to the visit function, called for each element.
 */
void dllist_visit(dllist l, void (*visit)(const void *));

/**
 * @brief Gets the data from a node.
 *
 * @param node The node to get the data from.
 * @return Pointer to the data, or NULL if node is NULL.
 */
const void* dllist_node_data(dllnode node);

/**
 * @brief Frees the memory allocated for the list.
 *
 * After calling this, the list pointer becomes invalid.
 *
 * @param l Pointer to the list to free.
 */
void dllist_free(dllist l);

#endif