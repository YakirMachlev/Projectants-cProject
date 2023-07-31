#pragma once

#include <stdio.h>
#include "asm_memory.h"

typedef enum
{
    NONE,
    UNKNOWN,
    NULL_POINTER,
    UNDEFINED_LABEL,
    UNKNOWN_COMMAND,
    INVALID_ARGUMENT,
    OVERFLOW,
    INVALID_FILE_PATH,
    NO_PARAMETERS,
    MISSING_ARGUMENTS,
    INVALID_ADDRESS_METHOD,
    EXTRANEOUS_TEXT,
    INVALID_INSTRUCTION,
    MISSING_COMMA,
    INVALID_DATA,
    MISSING_OPENING_QUOTES,
    MISSING_CLOSING_QUOTES,
    UNDEFINED_DIRECTIVE,
    MULTIPLE_LABEL_DEFINITIONS,
    INVALID_LABEL_NAME,
    CONTRARY_LABEL_ATTRIBUTES,
    NUMBER_OF_ERRORS /* Must be last */
} error_e;

/**
 * @brief prints an error with a line number
 * @file errors.h
 *
 * @param error an error
 * @return none
 */
void errors_print_line(error_e error);

/**
 * @brief sets the errors output file to the given file
 * @file errors.h
 * @param out the error file
 */
void errors_set_output(FILE* out);

/**
 * @brief increases the number of line of error
 * @file errors.h
 */
void errors_increase_lines();