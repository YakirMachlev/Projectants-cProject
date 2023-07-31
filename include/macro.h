#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <inttypes.h>
#include "linked_list.h"
#include "avl_tree.h"
#include "hash.h"

#define LINE_LENGTH 81
typedef avl_node_t* macro_t;

/**
 * @brief create the am file which contains the given file with
 * all of its macros opened
 * @file macro.h
 * 
 * @param current_file the original file that is being read
 * @param am_file the file with the macros opened
 */
void macro_write_am_file(FILE* current_file, FILE *am_file);