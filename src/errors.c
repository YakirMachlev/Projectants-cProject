#include "errors.h"

/**
 * Messages for all the errors, sorted by the order of the error in the error_e enum.
 */
static const char *error_messages[NUMBER_OF_ERRORS] = {
    "",                             /* NONE */
    "Unknown error",                /* UNKNOWN */
    "Null pointer",                 /* NULL_POINTER */
    "Undefined label",              /* UNDEFINED_LABEL */
    "Unknown command",              /* UNKNOWN_COMMAND */
    "Invalid argument",             /* INVALID_ARGUMENT */
    "Not enough memory",            /* OVERFLOW */
    "Invalid file path",            /* INVALID_FILE_PATH */
    "Not enough parameters",        /* NO_PARAMETERS */
    "Missing arguments",            /* MISSING_ARGUMENTS */
    "Invalid address method",       /* INVALID_ADDRESS_METHOD */
    "Extraneous text",              /* EXTRANEOUS_TEXT */
    "Invalid instruction",          /* INVALID_INSTRUCTION */
    "Missing comma",                /* MISSING_COMMA */
    "Invalid data" ,                /* INVALID_DATA */
    "Missing opening quotes",       /* MISSING_OPENING_QUOTES */
    "Missing closing quotes",       /* MISSING_CLOSING_QUOTES */
    "Undefined directive",          /* UNDEFINED_DIRECTIVE */
    "Multiple label definitions",   /* MULTIPLE LABEL DEFINITIONS */
    "Invalid label name",           /* INVALID_LABEL_NAME */
    "Contrary label attributes",    /* CONTRARY_LABEL_ATTRIBUTES */
};

#define CHECK_ERROR(error)                      \
    if (error < 0 || error >= NUMBER_OF_ERRORS) \
    {                                           \
        error = UNKNOWN;                        \
    }

static FILE* output;
static uint16_t lines = 1;

void errors_print_line(error_e error)
{
    CHECK_ERROR(error)
    printf("%04d\t%s\n", lines, error_messages[error]);
}

void errors_set_output(FILE *out)
{
    output = out;
}

void errors_increase_lines()
{
    ++lines;
}