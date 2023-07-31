#include "directive.h"

#define SKIP_SPACES(line)       \
    while (isspace(*(line++))); \
    --line;

static const char *directives[] = {".extern", ".string", ".entry", ".data", ""};

/**
 * @brief creates a hash value for the directives
 * 
 * @param word the directive
 * @return uint8_t the hash value
 */
static uint8_t directive_hash(char *word)
{
    static const uint8_t values[] = {
        3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 1, 4, 4, 4, 0, 4, 4};
    return values[((uint8_t)(word[2] - 'a')) % 26];
}

/**
 * @brief seperates the numbers from the data directive
 *
 * @param data the entire data in the data directive
 */
static void directive_analyze_data(char *data)
{
    int num;
    char *current;
    char scanned;
    bool is_valid;
    memory_t *memory;

    current = data;
    is_valid = true;
    memory = asm_memory_get_instance();
    
    SKIP_SPACES(current)
    while (is_valid && sscanf(current, "%d", &num) == 1)
    {
        asm_memory_push_data(memory, num);

        while (*(++current) && (!isspace(*current) && *current != ','));
        SKIP_SPACES(current)

        if (*current)
        {
            if (*current != ',')
            {
                is_valid = false;
                errors_print_line(MISSING_COMMA);
            }
            else
            {
                ++current;
                SKIP_SPACES(current)
            }
        }
    }
    if (sscanf(current, "%1s", &scanned) > 0)
    {
        errors_print_line(INVALID_DATA);
    }
}

/**
 * @brief seperate the characters from the string directive
 *
 * @param data the entire string in the string directive
 */
static void directive_analyze_string(char *data)
{
    char *current;
    memory_t *memory;

    SKIP_SPACES(data);
    if (*data != '"')
    {
        errors_print_line(MISSING_OPENING_QUOTES);
    }
    else
    {
        memory = asm_memory_get_instance();
        current = data;
        while (*(++current) && *current != '"')
        {
            asm_memory_push_data(memory, *current);
        }
        asm_memory_push_data(memory, '\0');
        if (*current != '"')
        {
            errors_print_line(MISSING_CLOSING_QUOTES);
        }
    }
    
}

void directive_handle(char *line)
{
    uint8_t d_hash;
    char directive[9];

    SKIP_SPACES(line);
    sscanf(line, "%9s", directive);
    d_hash = directive_hash(directive);

    if (d_hash == DATA && strcmp(directive, directives[d_hash]) == 0)
    {
        directive_analyze_data(line + 5);
    }
    else if (d_hash == STRING && strcmp(directive, directives[d_hash]) == 0)
    {
        directive_analyze_string(line + 7);
    }
    else if ((d_hash == ENTRY || d_hash == EXTERN) && strcmp(directive, directives[d_hash]) == 0)
    {
        labels_table_add_entry_or_extern_labels(labels_table_get_instance(), line + 7, d_hash == ENTRY);
    }
    else
    {
        errors_print_line(UNDEFINED_DIRECTIVE);
    }
}

directive_e directive_get(char *directive)
{
    uint8_t d_hash;
    d_hash = directive_hash(directive);
    return (strcmp(directive, directives[d_hash]) == 0) ? d_hash : INVALID_DIRECTIVE;
}
