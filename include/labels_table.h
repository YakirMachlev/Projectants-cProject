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
#include "label.h"
#include "directive.h"
#include "asm_output.h"
#include "asm_memory.h"

typedef struct labels_table_t labels_table_t;

/**
 * @brief returns the instance of the labels table
 * @file labels_table.h
 * 
 * @return label_table_t* the labels table
 */
labels_table_t *labels_table_get_instance();

/**
 * @brief checks if a given word is a label by searching it's hash
 * value in the avl tree of the labels table 
 * @file labels_table.h
 * 
 * @param table the labels table
 * @param word the word that it tries to find
 * @return avl_node_t* the node of the label (NULL if not found)
 */
label_t *labels_table_does_label_exist(labels_table_t *table, char* word);

/**
 * @brief searches a label (by it's name) in the labels table. If the label
 * doesn't exist, it will create the label and insert it to the labels table
 * @file labels_table.h
 * 
 * @param table the labels table
 * @param symbol the label symbol
 * @return label_t* the found/created label
 */
label_t *labels_table_get_forced_label(labels_table_t *table, char* symbol);

/**
 * @brief returns the number of extern labels in the table
 * @file labels_table.h
 * 
 * @param table lables table
 * @return number extern labels in the table 
 */
int labels_table_get_extern_count(labels_table_t *table);

/**
 * @brief returns the number of entry labels in the table
 * @file labels_table.h
 * 
 * @param table lables table
 * @return number entry labels in the table 
 */
int labels_table_get_entry_count(labels_table_t *table);

/**
 * @brief adds a label to the table (or modifies the label in case
 * it is already apparent in the avl tree)
 * @file labels_table.h
 *
 * @param table the labels table
 * @param line the line in which there is a label definition
 * @param is_entry is the lable of type entry
 */
void labels_table_add_entry_or_extern_labels(labels_table_t *table, char *line, bool is_entry);

/**
 * @brief checks if a label is defined in a given line and adds the label
 * to the avl tree that is in the labels table
 * @file labels_table.h
 *
 * @param line the line that is being checked
 * @param table the labels table
 */
void labels_table_add_if_definition(char **line, labels_table_t *table);

/**
 * @brief opens the file (ext/ent) according to the label's attributes and
 * writes the labels information in the proper format based on the file type
 * @file labels_table.h
 * 
 * @param table the labels table
 * @param ext_file the file containing the external labels
 * @param ent_file the file containing the entry labels
 */
void labels_table_write_ext_and_ent_proxy(labels_table_t *table, FILE *ext_file, FILE *ent_file);

/**
 * @brief inserts the labels to the memory structure
 * @file labels_table.h
 * 
 * @param table the labels table
 * @param memory the memory structure
 */
void labels_table_insert_labels_to_memory_proxy(labels_table_t *table, memory_t *memory);

/**
 * @brief prints the labels table
 * @file labels_table.h
 * 
 * @param root the labels table
 */
void labels_table_print(labels_table_t *table);

/**
 * @brief checks that all of the labels are defined correctly
 * @file labels_table.h
 * 
 * @param table the labels table
 */
void labels_table_check_labels_validity_proxy(labels_table_t *table);

/**
 * @brief destroys the entire struct of the labels table
 * @file labels_table.h
 *
 * @param table the table that is being freed
 */
void labels_table_destroy(labels_table_t *table);
