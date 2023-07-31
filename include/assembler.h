#pragma once

#include "errors.h"
#include "macro.h"
#include "labels_table.h"
#include "asm_line.h"
#include "asm_language.h"
#include "directive.h"
#include "asm_memory.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/**
 * @brief translates the given assembly file to machine code (creates .ob,
 * .ent, .ext and .err files)
 * @file assembler.h
 * 
 * @param file_name the name of the file that is being processed
 */
void assembler_on_file(char *file_name);