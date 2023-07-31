#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "avl_tree.h"
#include "hash.h"
#include "errors.h"
#include "asm_language.h"
#include "linked_list.h"

#define LABEL_MAX_LENGTH 31
#define LINE_LENGTH 81

#define CODE_FLAG 1
#define DATA_FLAG 2
#define EXTERN_FLAG 4
#define ENTRY_FLAG 8

typedef struct label_t label_t;

/**
 * @brief gets the symbol of a given label
 * 
 * @param label the label that contains the symbol
 * @return char* the symbol of the label
 */
char* label_get_symbol(label_t *label);

/**
 * @brief gets a linked list of the lines in which the given label is found
 * @file label.h
 * 
 * @param label the label
 * @return linked_list_node_t* the label lines
 */
linked_list_node_t *label_get_lines(label_t *label);

/**
 * @brief inserts a line to the labels lines
 * @file label.h
 * 
 * @param label the label to which it inserts the line
 * @param line the line num
 */
void label_insert_line(label_t *label, int line);

/**
 * @brief returns the base address of the label
 * @file label.h
 * 
 * @param label the label
 * @return int the base address of the label
 */
int label_get_base_address(label_t *label);

/**
 * @brief returns the offset of the label
 * @file label.h
 * 
 * @param label the label
 * @return unsigned int the offset of the label
 */
unsigned int label_get_offset(label_t *label);

/**
 * @brief returns the attibutes of the label
 * @file label.h
 * 
 * @param label the label
 * @return unsigned int the attributes of the label
 */
int label_get_attributes(label_t *label);

/**
 * @brief sets the base address of the label to the given base offset
 * @file label.h
 * 
 * @param label the label
 * @param base_address the given base address
 */
void label_set_base_address(label_t *label, int base_address);

/**
 * @brief sets the offset of the label to a given offset
 * @file label.h
 * 
 * @param label the label
 * @param offset the given offset
 */
void label_set_offset(label_t *label, unsigned int offset);

/**
 * @brief sets the attributes of the label to given attributes
 * @file label.h
 * 
 * @param label the label
 * @param attributes the given attributes
 */
void label_set_attributes(label_t *label, int attributes);

/**
 * @brief add an attribute to a given label's attributes
 * @file label.h
 * 
 * @param label the label
 * @param attribute the added attribute
 */
void label_add_attribute(label_t *label, int attribute);

/**
 * @brief checks if there is a label definition in a given line
 *
 * @param line the line that is being checked
 * @return bool is there a label in the line
 */
bool label_check_if_definition(char *label_name);

/**
 * @brief creates a label object
 *
 * @param symbol the name of the label
 * @return label_t* the label that is created
 */
label_t *label_create(char *symbol);

/**
 * @brief destroys the label lines
 * 
 * @param label a label
 */
void label_destroy_lines(label_t *label);