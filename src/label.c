#include "label.h"

struct label_t
{
    char symbol[LABEL_MAX_LENGTH];
    linked_list_node_t *lines;
    int base_address;
    unsigned int offset : 4;
    int attributes : 4;
};

#define IS_VALID_LABEL(label_name) \
    !asm_language_is_saved_word(label_name) && isalpha(*label_name)

char *label_get_symbol(label_t *label)
{
    return label->symbol;
}

linked_list_node_t *label_get_lines(label_t *label)
{
    return label->lines;
}

void label_insert_line(label_t *label, int line_num)
{
    int *line_num_ptr;

    line_num_ptr = (int*)malloc(sizeof(int));
    assert("Memory allocation failed" && line_num_ptr != NULL);
    *line_num_ptr = line_num;

    linked_list_push(&(label->lines));
    linked_list_set_info(label->lines, line_num_ptr);
}

void label_destroy_lines(label_t *label)
{
    linked_list_node_t **lines = &(label->lines);
    int *temp;

    while (*lines)
    {
        temp = linked_list_get_info(*lines);
        linked_list_pop(lines);
        free(temp);
    }
}

int label_get_base_address(label_t *label)
{
    return label->base_address;
}

unsigned int label_get_offset(label_t *label)
{
    return label->offset;
}

int label_get_attributes(label_t *label)
{
    return label->attributes;
}

void label_set_base_address(label_t *label, int base_address)
{
    label->base_address = base_address;
}

void label_set_offset(label_t *label, unsigned int offset)
{
    label->offset = offset;
}

void label_set_attributes(label_t *label, int attributes)
{
    label->attributes = attributes;
}

void label_add_attribute(label_t *label, int attribute)
{
    label->attributes |= attribute;
}


label_t *label_create(char *symbol)
{
    label_t *new_label;

    new_label = (label_t *)malloc(sizeof(label_t));
    assert("Couldn't allocate new node for the labels table\n" && new_label != NULL);
    strcpy(new_label->symbol, symbol);
    new_label->lines = NULL;
    new_label->base_address = -1;
    new_label->offset = 0;
    new_label->attributes = 0;

    return new_label;
}

bool label_check_if_definition(char *label_name)
{
    bool is_valid;
    char length;

    length = strlen(label_name);
    is_valid = label_name[length - 1] == ':';
    if (is_valid)
    {
        label_name[length - 1] = '\0';
    }
    return is_valid;
}