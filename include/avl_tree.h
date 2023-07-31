#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct avl_node_t avl_node_t;

/**
 * @brief return the key of the given node
 * @file avl_tree.h
 *
 * @param node a node of an avl tree
 * @return int the key of the node
 */
unsigned long avl_tree_get_key(avl_node_t *node);

/**
 * @brief return the data of the given node
 * @file avl_tree.h
 *
 * @param node a node of an avl tree
 * @return void* the data of the node
 */
void *avl_tree_get_data(avl_node_t *node);

/**
 * @brief return the left child of the given node
 * @file avl_tree.h
 *
 * @param node the father node
 * @return avl_node_t* the left child
 */
avl_node_t *avl_tree_get_left_child(avl_node_t *node);

/**
 * @brief return the right child of the given node
 * @file avl_tree.h
 *
 * @param node the father node
 * @return avl_node_t* the right child
 */
avl_node_t *avl_tree_get_right_child(avl_node_t *node);

/**
 * @brief finds the height of a received node
 *
 * @param n a node in the avl tree
 * @return int the height of the tree
 */
int avl_tree_get_height(avl_node_t *node);

/**
 * @brief sets the key field of the given node to the given key
 * @file avl_tree.h
 *
 * @param node a node of an avl tree
 * @param key the key that will be inserted to the node
 */
void avl_tree_set_key(avl_node_t *node, unsigned long key);

/**
 * @brief sets the data field of the given node to the given data
 * @file avl_tree.h
 *
 * @param node a node of an avl tree
 * @param data the data that will be inserted to the node
 */
void avl_tree_set_data(avl_node_t *node, void *data);

/**
 * @brief creates a node for an avl tree
 * @file avl_tree.h
 *
 * @param key the node key
 * @return Node* the created node
 */
avl_node_t *avl_tree_create_node(unsigned long key);

/**
 * @brief inserts a node into an avl tree
 * @file avl_tree.h
 *
 * @param root the root of the avl tree
 * @param node the node that in being inserted
 */
void avl_tree_insert_node(avl_node_t **root, avl_node_t *node);

/**
 * @brief searches the avl tree according to a given key
 * @file avl_tree.h
 *
 * @param root the root of the avl tree
 * @param key a key by which we find the node
 * @return Node* the node that was found
 */
avl_node_t *avl_tree_search_node(avl_node_t *root, unsigned long key);

/**
 * @brief deletes a node from a given avl tree according to its key
 * @file avl_tree.h
 *
 * @param root the root of the avl tree
 * @param key the key by which it deletes the node
 * @return Node* the deleted node
 */
avl_node_t *avl_tree_delete_node(avl_node_t *root, unsigned long key);

/**
 * @brief frees all of the nodes in the given avl tree
 * @file avl_tree.h
 *
 * @param root the root of the avl tree
 */
void avl_tree_destroy(avl_node_t *root);

/**
 * @brief prints the entire tree in level order
 * @file avl_tree.h
 *
 * @param root the root of the avl tree that it prints
 */
void avl_tree_print_level_order(avl_node_t *root);
