/**
 * @file binary_tree.h
 * @brief A simple generic binary tree implementation.
 * @details A <a href="https://en.wikipedia.org/wiki/Binary_tree">Binary Tree</a>
 * is a hierarchical data structure where each node has at most two children.
 * The tree can be traversed in various ways, such as in-order, pre-order, and post-order.
 * Elements can be inserted and searched for in the tree.
 
 * This tree does not impose any ordering on the tree.
 * For this you would need a binary search tree.
 * 
 * Currently, it is not possible to delete nodes from the tree.
 * 
 * Don't forget to free the tree with bintree_free() when you're done to avoid memory leaks.
 * 
 * @see binary_tree.c for implementation details.
 * @example
 * @code
 * int main(void)
 * {
 *
 *         char two = '2';
 *         char plus = '+';
 *         char three = '3';
 *         char times = '*';
 *         char four = '4';
 * 
 *         bintree_t bintree = bintree_create(3, sizeof(char));
 *         btnode_t root = bintree_create_root(bintree, &plus);
 * 
 *         bintree_insert_left(bintree, root, &two);
 *         btnode_t times_node = bintree_insert_right(bintree, root, &times);
 *         bintree_insert_left(bintree, times_node, &three);
 *         bintree_insert_right(bintree, times_node, &four);
 * 
 *         bintree_visit_in_order(bintree, root, visit_tree);
 *         bintree_free(bintree);
 * 
 *         return EXIT_SUCCESS;
 * }
 *
 * static void visit_tree(const void *data)
 * {
 *         printf("%c", *(const char *)data);
 * }
 * @endcode
 * The sample above creates a binary tree for the expression 2 + 3 * 4,
 * where 3 * 4 has to be evaluated before adding it to two.
 */

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdbool.h>
#include <stddef.h>

#include "util/chunk.h"


typedef struct binary_tree *bintree_t;
typedef struct binary_tree_node *btnode_t;


/**
 * @brief Creates a new binary tree.
 *
 * Allocates memory for a new binary tree with the specified initial capacity and element size.
 *
 * @param initial_capacity Initial capacity of the tree (number of elements).
 * @param element_size Size of each element in bytes.
 * @return Pointer to the new binary tree, or NULL if allocation failed or element_size is 0.
 */
bintree_t bintree_create(size_t initial_capacity, size_t element_size);

/**
 * @brief Creates the root node.
 * @param element Pointer to the element to insert.
 * @return Pointer to the created root node, or NULL if allocation failed or element is 0.
 */
btnode_t bintree_create_root(bintree_t bintree, const void *element);

/**
 * @brief Gets the root node of the supplied binary tree.
 *
 * @param bintree Pointer to the binary tree.
 * @return Root node of the supplied binary tree.
 *  NULL is returned when no elements have been added to the tree.
 */
btnode_t bintree_get_root(bintree_t bintree);

/**
 * @brief Creates a new node to the left of the supplied node, provided no left node exists.
 * 
 *
 * @param bintree Pointer to the binary tree.
 * @param btnode_t Pointer to the node where the new is added.
 * @param element Pointer to the element to insert.
 * @return The created node or NULL when no node was inserted.
 */
btnode_t bintree_insert_left(bintree_t bintree, btnode_t node, const void *element);

/**
 * @brief Creates a new node to the right of the supplied node, provided no right node exists.
 *
 * @param bintree Pointer to the binary tree.
 * @param btnode_t Pointer to the node where the new is added.
 * @param element Pointer to the element to insert.
 * @return The created node or NULL when no node was inserted.
 */
btnode_t bintree_insert_right(bintree_t bintree, btnode_t node, const void *element);

/**
 * @brief Visits all nodes, starting with the supplied node, in pre-order.
 *
 * @param bintree Pointer to the binary tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each node.
 */
void bintree_visit_pre_order(bintree_t bintree, btnode_t node, void (*visit)(const void *));

/**
 * @brief Visits all nodes, starting with the supplied node, in order.
 *
 * @param bintree Pointer to the binary tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each node.
 */
void bintree_visit_in_order(bintree_t bintree, btnode_t node, void (*visit)(const void *));

/**
 * @brief Visits all nodes, starting with the supplied node, in post order.
 *
 * @param bintree Pointer to the binary tree.
 * @param node The node to start from.
 * @param visit Pointer to the visit function, called for each node.
 */
void bintree_visit_post_order(bintree_t bintree, btnode_t node, void (*visit)(const void *));

/**
 * @brief Frees the memory allocated for the binary tree.
 *
 * @param bintree Pointer to the binary tree to free.
 */
void bintree_free(bintree_t t);

#endif