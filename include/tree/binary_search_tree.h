/**
 * @file binary_tree.h
 * @brief A simple generic binary search tree implementation.
 * @details A binary search tree is a hierarchical data structure where each node has at most two children.
 * The tree can be traversed in various ways, such as in-order, pre-order, and post-order.
 * Elements can be inserted and searched for in the tree.
 * 
 * Don't forget to free the tree with bstree_free() when you're done to avoid memory leaks.
 * @example
 * @code
 * @endcode
 */

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>
#include <stddef.h>


typedef struct binary_search_tree *bstree;

/**
 * @brief Creates a new binary search tree.
 *
 * Allocates memory for a new binary search tree with the specified initial capacity and element size.
 *
 * @param initial_capacity Initial capacity of the tree (number of elements).
 * @param element_size Size of each element in bytes.
 * @return Pointer to the new binary search tree, or NULL if allocation failed or element_size is 0.
 */
bstree bstree_create(size_t initial_capacity, size_t element_size);

/**
 * @brief Inserts an element into the binary search tree.
 *
 * @param bintree Pointer to the binary search tree.
 * @param element Pointer to the element to insert.
 * @return true if successful, false if allocation failed or tree is NULL.
 */
bool bstree_insert(bstree t, const void *element);

/**
 * @brief Frees the memory allocated for the binary search tree.
 *
 * @param bstree Pointer to the binary search tree to free.
 */
void bstree_free(bstree t);

#endif