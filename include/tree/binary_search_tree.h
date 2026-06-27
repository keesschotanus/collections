/**
 * @file binary_search_tree.h
 * @brief A simple generic binary search tree implementation.
 * @details A <a href="https://en.wikipedia.org/wiki/Binary_search_tree">Binary Search Tree</a>
 * is a hierarchical data structure where each node has at most two children.
 * The tree can be traversed in various ways, such as in-order, pre-order, and post-order.
 * Elements can be inserted and searched for in the tree.
 * 
 * Don't forget to free the tree with bstree_free() when you're done to avoid memory leaks.
 * 
 * @see binary_search_tree.c for implementation details.
 * @example
 * @code
 * @endcode
 */

// TODO add example

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>
#include <stddef.h>

#include "tree/binary_tree.h"


typedef struct binary_search_tree *bstree_t;


/**
 * @brief Creates a new binary search tree.
 *
 * Allocates memory for a new binary search tree with the specified initial capacity and element size.
 *
 * @param initial_capacity Initial capacity of the tree (number of elements).
 * @param element_size Size of each element in bytes.
 * @param compare Function pointer to the comparison function.
 * @return Pointer to the new binary search tree, or NULL if allocation failed or element_size is 0.
 */
bstree_t bstree_create(size_t initial_capacity, size_t element_size, int (*compare)(const void *, const void *));

/**
 * @brief Gets the root node of the supplied binary search tree.
 *
 * @param bst Pointer to the binary search tree.
 * @return Root node of the supplied binary search tree
 *  NULL is returned when no elements have been added to the tree.
 */
btnode_t bstree_get_root(bstree_t bst);

/**
 * @brief Inserts an element into the binary search tree.
 *
 * @param bst Pointer to the binary search tree.
 * @param element Pointer to the element to insert.
 * @return true if successful, false if allocation failed or tree is NULL.
 */
bool bstree_insert(bstree_t bst, const void *element);

/**
 * @brief Searches the binary search tree for an element.
 *
 * @param bst Pointer to the binary search tree.
 * @param element Pointer to the element to search for.
 * @return true if the element is present, false otherwise.
 */
bool bstree_search(bstree_t bst, const void *element);

/**
 * @brief Visits all nodes, starting with the supplied node, in pre-order.
 *
 * @param bst Pointer to the binary search tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each element.
 */
void bstree_visit_pre_order(bstree_t bst, btnode_t node, void (*visit)(const void *));

/**
 * @brief Visits all nodes, starting with the supplied node, in order.
 *
 * @param bst Pointer to the binary search tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each element.
 */
void bstree_visit_in_order(bstree_t bst, btnode_t node, void (*visit)(const void *));

/**
 * @brief Visits all nodes, starting with the supplied node, in post order.
 *
 * @param bst Pointer to the binary search tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each element.
 */
void bstree_visit_post_order(bstree_t bst, btnode_t node, void (*visit)(const void *));

/**
 * @brief Get the number of nodes in the tree.
 * 
 * @param bst The tree to get the size of.
 * @return size_t The number of nodes in the tree.
 */
size_t bstree_size(bstree_t bst);

/**
 * @brief Frees the memory allocated for the binary search tree.
 *
 * @param bstree Pointer to the binary search tree to free.
 */
void bstree_free(bstree_t t);

#endif