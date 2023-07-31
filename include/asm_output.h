#pragma once

#include <stdio.h>
#include "asm_language.h"
#include "label.h"
#include "argument.h"
#include "asm_memory.h"

/**
 * @brief writes the label information to the extern file in the
 * required format
 * 
 * @param ext_file the .ext file 
 * @param label the label
 */
void asm_output_extern_label(FILE *ext_file, label_t *label);

/**
 * @brief writes the label information to the entry file in the
 * required format
 * 
 * @param ext_file the .ent file 
 * @param label the label
 */
void asm_output_entry_label(FILE *ent_file, label_t *label);

/**
 * @brief prints an object file
 * 
 * @param ob_file an object file
 * @param memory an asm memory
 */
void asm_output_ob_file(FILE *ob_file, memory_t *memory);