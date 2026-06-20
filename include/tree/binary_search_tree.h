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

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>
#include <stddef.h>

#include "util/chunk.h"


typedef struct binary_search_tree binary_search_tree;
typedef struct binary_search_tree_node binary_search_tree_node;

typedef struct binary_search_tree *bstree_t;
typedef struct binary_search_tree_node *bstnode_t;


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
 * @param bstree Pointer to the binary search tree.
 * @return Root node of the supplied binary tree
 *  NULL is returned when no elements have been added to the tree.
 */
bstnode_t bstree_get_root(bstree_t bst);

/**
 * @brief Inserts an element into the binary search tree.
 *
 * @param bintree Pointer to the binary search tree.
 * @param element Pointer to the element to insert.
 * @return true if successful, false if allocation failed or tree is NULL.
 */
bool bstree_insert(bstree_t t, const void *element);

/**
 * @brief Visits all nodes, starting with the supplied node, in order.
 *
 * @param bintree Pointer to the binary search tree.
 * @param element Pointer to the node to start from .
 */
void bstree_visit_in_order(bstree_t bst, bstnode_t node);

/**
 * @brief Frees the memory allocated for the binary search tree.
 *
 * @param bstree Pointer to the binary search tree to free.
 */
void bstree_free(bstree_t t);

#endif