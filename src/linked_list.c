#include "linked_list.h"

struct linked_list_node_t
{
	void *info;
	linked_list_node_t *next_address;
};

void *linked_list_get_info(linked_list_node_t *node)
{
	return node->info;
}

linked_list_node_t *linked_list_get_next_address(linked_list_node_t *node)
{
	return node->next_address;
}

void linked_list_set_info(linked_list_node_t *node, void *info)
{
	node->info = info;
}

void linked_list_set_next_address(linked_list_node_t *node, linked_list_node_t *next)
{
	node->next_address = next;
}

void linked_list_push(linked_list_node_t **manager)
{
	linked_list_node_t *new_node;

	new_node = (linked_list_node_t *)malloc(sizeof(linked_list_node_t));
	assert("Couldn't allocate new node for the linked list\n" && new_node != NULL);
	new_node->next_address = *manager;
	*manager = new_node;
}

void linked_list_insert_after_node(linked_list_node_t *node)
{
	linked_list_node_t *new_node;

	new_node = (linked_list_node_t *)malloc(sizeof(linked_list_node_t));
	assert("Couldn't allocate new node for the linked list\n" && new_node != NULL);
	new_node->next_address = node->next_address;
	node->next_address = new_node;
}

void linked_list_pop(linked_list_node_t **manager)
{
	linked_list_node_t *temp_node;
	temp_node = *manager;
	*manager = (*manager)->next_address;
	free(temp_node);
}

void linked_list_delete_after_node(linked_list_node_t *node)
{
	linked_list_node_t *temp_node;
	temp_node = node->next_address;
	node->next_address = temp_node->next_address;
	free(temp_node);
}

void linked_list_destroy(linked_list_node_t **manager)
{
	linked_list_node_t **start;
	start = manager;

	while (*start)
	{
		linked_list_pop(start);
	}
}