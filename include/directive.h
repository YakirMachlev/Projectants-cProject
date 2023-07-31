#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include "labels_table.h"
#include "errors.h"

typedef enum
{
    EXTERN,
    STRING,
    ENTRY,
    DATA,
    INVALID_DIRECTIVE
} directive_e;

/**
 * @brief returns the directive type
 * @file directive.h
 * 
 * @param directive the directive
 * @return directive_e 
 */
directive_e directive_get(char* directive);

/**
 * @brief handles a directive
 * @file directive.h
 * 
 * @param line a directive line
 */
void directive_handle(char *line);
