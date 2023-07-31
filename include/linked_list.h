#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct linked_list_node_t linked_list_node_t;

/**
 * @brief returns the info of the given node
 *
 * @param node a node from the linked list
 * @return char* the node's info
 */
void *linked_list_get_info(linked_list_node_t *node);

/**
 * @brief returns a pointer to the node that is after a given node in
 * the linked list
 *
 * @param node a node from the linked list
 * @return linked_list_node_t* the node after the given node
 */
linked_list_node_t *linked_list_get_next_address(linked_list_node_t *node);

/**
 * @brief sets the info field of a given node to the given info
 *
 * @param node the node that is mdified
 * @param info the info that is inserted to the node
 */
void linked_list_set_info(linked_list_node_t *node, void *info);

/**
 * @brief sets the next_address field of a given node to the given address
 *
 * @param node the node that is mdified
 * @param next the node that will be linked to the given node
 */
void linked_list_set_next_address(linked_list_node_t *node, linked_list_node_t *next);

/**
 * @brief inserts a node from the start of the linked list
 *
 * @param manager the manager of the linked list
 */
void linked_list_push(linked_list_node_t **manager);

/**
 * @brief inserts a node to the linked list after a given node
 *
 * @param node that node that after which the new node will be inserted
 */
void linked_list_insert_after_node(linked_list_node_t *node);

/**
 * @brief removes a node from the start of the linked list
 *
 * @param manager a pointer to the manager of the linked list
 */
void linked_list_pop(linked_list_node_t **manager);

/**
 * @brief removes a node from the linked list after a given node
 *
 * @param node that node that after which the new node will be removed
 */
void linked_list_delete_after_node(linked_list_node_t *node);

/**
 * @brief frees all of the allocated space of the linked list
 *
 * @param manager the manager of the linkde list
 */
void linked_list_destroy(linked_list_node_t **manager);