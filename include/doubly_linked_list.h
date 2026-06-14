/**
 * @file doubly_linked_list.h
 * @brief Doubly linked list implementation.
 * @details A doubly linked list is a data structure that can grow dynamically.
 * The list can hold elements of any type, specified by the element size at creation.
 * 
 * A doubly linked list is a data structure that consists of nodes where each node contains a
 * pointer to the next node and a pointer to the previous node.
 * Insert and delete operations are very fast.
 * Unlike the dynamic list, elements can be inserted at any position in the list.
 * When removing elements, memory is reused.
 * 
 * This list can pose as a stack by using dllist_push() and dllist_pop().
 * 
 * Don't forget to free the list with list_free() when you're done to avoid memory leaks.
 * @example
 * @code
 * #include "doubly_linked_list.h"
 *
 * static int compare_ints(const void *a, const void *b)
 * {
 *          const int *int_a = (const int *)a;
 *          const int *int_b = (const int *)b;
 * 
 *          return (*int_a > *int_b) - (*int_a < *int_b);
 * }
 * 
 * int main(void)
 * {
 *         dllist l = dllist_create(sizeof(int));
 *         // Add at beginning
 *         dllist_insert_before(l, NULL, &(int){1});
 *         
 *         // Find the node with a value of 1
 *         dllnode node = dllist_find(l, &(int){1}, compare_ints);
 *
 *         // Insert a node after the found node
 *         dllist_insert_after(l, node, &(int){2});
 *
 *         // Remove the last node from the list
 *         dllist_remove(l, dllist_last(l));
 *         
 *         dllist_free(l);
 *
 *         return 0;
 * }
 * @endcode
 *
 * In the example above, a doubly linked list is created to hold elements of type int.
 * Other types work similarly.
 * The first integer is stored at the head of the list.
 * You can also insert elements to the tail and before or after a specific node
 * that you locate using the dllist_find() function.
 * 
 * Note that you need a function to compare two elements of the same type.
 * In the example compare_ints is used.
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
 * @brief Treats this list as a stack and pushes an element to the end.
 * 
 * @param l Pointer to the list.
 * @param element Pointer to the element to push.
 * @return true if successful, false if allocation failed or list is NULL.
 */
bool dllist_push(dllist l, const void *element);

/**
 * @brief Treats this list as a stack and pops an element from the end.
 * 
 * @param l Pointer to the list.
 * @return Pointer to the popped data, or NULL if the list is empty or NULL.
 */
void *dllist_pop(dllist l);

/**
 * @brief Treats this list as a stack and peeks at the top element.
 * 
 * @param l Pointer to the list.
 * @return Pointer to the popped data, or NULL if the list is empty or NULL.
 */
void *dllist_peek(dllist l);

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