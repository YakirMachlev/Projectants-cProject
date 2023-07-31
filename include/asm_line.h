#pragma once

#include "labels_table.h"
#include "asm_language.h"
#include "argument.h"
#include "errors.h"
#include "directive.h"
#include <inttypes.h>

/**
 * @brief analyzes asm line
 * @file asm_line.h
 * 
 * @param line an assembly line
 */
void asm_line_analyze(char *line);
