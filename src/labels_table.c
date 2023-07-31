#include "labels_table.h"

#define IS_VALID_LABEL(label_name) \
!asm_language_is_saved_word(label_name) && isalpha(*label_name)

struct labels_table_t
{
    avl_node_t *root;
    int extern_count;
    int entry_count;
};

static labels_table_t *label_table_instance;

/**
 * @brief allocated and initializes a labels table
 *
 * @return a labels table
 */
static labels_table_t *labels_table_init()
{
    labels_table_t *new_table;

    new_table = (labels_table_t *)malloc(sizeof(labels_table_t));
    new_table->root = NULL;
    new_table->extern_count = 0;
    new_table->entry_count = 0;

    return new_table;
}

labels_table_t *labels_table_get_instance()
{
    if (label_table_instance == NULL)
    {
        label_table_instance = labels_table_init();
    }
    return label_table_instance;
}

int labels_table_get_extern_count(labels_table_t *table)
{
    return table->extern_count;
}

int labels_table_get_entry_count(labels_table_t *table)
{
    return table->entry_count;
}

label_t *labels_table_does_label_exist(labels_table_t *table, char *word)
{
    return avl_tree_get_data(avl_tree_search_node(table->root, hash(word)));
}

/**
 * @brief creates an empty node and inserts it to the avl tree according
 * to the label name
 *
 * @param root the root of the avl tree
 * @param label_hash the label hash
 * @return Node* the created node
 */
static avl_node_t *labels_table_insert_node(avl_node_t **root, unsigned long label_hash)
{
    avl_node_t *label_node;
    label_node = avl_tree_create_node(label_hash);
    avl_tree_insert_node(root, label_node);
    return label_node;
}

label_t *labels_table_get_forced_label(labels_table_t *table, char* symbol)
{
    label_t *label;
    avl_node_t *added_label_node;
    unsigned long l_hash;

    l_hash = hash(symbol);
    label = avl_tree_get_data(avl_tree_search_node(table->root, l_hash));
    if (!label)
    {
        label = label_create(symbol);
        label_set_base_address(label, -1);
        added_label_node = labels_table_insert_node(&(table->root), l_hash);
        avl_tree_set_data(added_label_node, label);
    }
    return label;
}

/**
 * @brief adds a label to the table (or modifies the label in case
 * it is already apparent in the avl tree)
 *
 * @param line the line in which there is a label definition
 * @param address the address of the label
 * @param table the labels table
 * @param segment_flag segment flag
 */
static void labels_table_add_definition_labels(char *label_name, int address, labels_table_t *table, uint8_t segment_flag)
{
    label_t *added_label;
    avl_node_t *added_label_node;
    unsigned long label_hash;

    label_hash = hash(label_name);
    address += START_IC_VALUE;
    if ((added_label_node = avl_tree_search_node(table->root, label_hash)))
    {
        added_label = avl_tree_get_data(added_label_node);
    }
    else
    {
        added_label_node = labels_table_insert_node(&(table->root), label_hash);
        added_label = label_create(label_name);
        avl_tree_set_data(added_label_node, added_label);
    }

    if (label_get_base_address(added_label) == -1)
    {
        label_set_offset(added_label, address % 16);
        label_set_base_address(added_label, address - label_get_offset(added_label));
        label_add_attribute(added_label, segment_flag);
    }
    else
    {
        errors_print_line(MULTIPLE_LABEL_DEFINITIONS);
    }
}

void labels_table_add_if_definition(char **line, labels_table_t *table)
{
    char label_name[LABEL_MAX_LENGTH + 1];
    char word[9];
    int label_length;
    directive_e dir;
    memory_t *memory;
    uint8_t segment_flag;

    sscanf(*line, "%s%n%9s", label_name, &label_length, word);

    if (label_check_if_definition(label_name) && IS_VALID_LABEL(label_name))
    {
        memory = asm_memory_get_instance();
        dir = directive_get(word);
        segment_flag = (dir == DATA || dir == STRING) ? DATA_FLAG : CODE_FLAG;
        labels_table_add_definition_labels(label_name, asm_memory_get_pc(memory), table, segment_flag);

        *line = *line + label_length;
    }
}

void labels_table_add_entry_or_extern_labels(labels_table_t *table, char *line, bool is_entry)
{
    avl_node_t *added_label_node;
    label_t *added_label;
    unsigned long label_hash;
    char label_name[LABEL_MAX_LENGTH];
    char scanned;
    uint8_t attributes;

    scanned = sscanf(line, "%s", label_name);
    if (scanned && IS_VALID_LABEL(label_name))
    {
        label_hash = hash(label_name);
        if ((added_label_node = avl_tree_search_node(table->root, label_hash)))
        {
            added_label = avl_tree_get_data(added_label_node);
        }
        else
        {
            added_label_node = labels_table_insert_node(&(table->root), label_hash);
            added_label = label_create(label_name);

            label_set_base_address(added_label, is_entry ? -1 : 0);
            avl_tree_set_data(added_label_node, added_label);
        }

        attributes = label_get_attributes(added_label);
        if (is_entry && !(attributes & EXTERN_FLAG))
        {
            ++(table->entry_count);
            label_add_attribute(added_label, ENTRY_FLAG);
        }
        else if (!is_entry && !(attributes & EXTERN_FLAG))
        {
            ++(table->extern_count);
            label_set_attributes(added_label, EXTERN_FLAG);
            label_set_base_address(added_label, 0);
        }
        else 
        {
            errors_print_line(CONTRARY_LABEL_ATTRIBUTES);
        }
    }
    else
    {
        if (!scanned)
        {
            errors_print_line(MISSING_ARGUMENTS);
        }
        else
        {
            errors_print_line(INVALID_LABEL_NAME);
        }
    }
}

/**
 * @brief writes the labels information in the proper file and format based
 * on the label type
 * @file labels_table.h
 * 
 * @param root the root of the labels table (avl tree)
 * @param ext_file the file containing the external labels
 * @param ent_file the file containing the entry labels
 */
static void labels_table_write_ext_and_ent(avl_node_t *root, FILE *ext_file, FILE *ent_file)
{
    label_t *label;
    int attribute;

    if (!root)
        return;

    label = avl_tree_get_data(root);
    attribute = label_get_attributes(label);

    if (attribute & EXTERN_FLAG)
    {
        asm_output_extern_label(ext_file, label);
    }
    else if (attribute & ENTRY_FLAG)
    {
        asm_output_entry_label(ent_file, label);
    }

    labels_table_write_ext_and_ent(avl_tree_get_left_child(root), ext_file, ent_file);
    labels_table_write_ext_and_ent(avl_tree_get_right_child(root), ext_file, ent_file);
}

void labels_table_write_ext_and_ent_proxy(labels_table_t *table, FILE *ext_file, FILE *ent_file)
{
    if (table->entry_count)
    {
        assert("Entry file isn't open" && ent_file);
    }
    else if (table->extern_count)
    {
        assert("Extern file isn't open" && ext_file);
    }
    labels_table_write_ext_and_ent(table->root, ext_file, ent_file);
}

/**
 * @brief inserts the labels to the memory structure
 * 
 * @param root the root of the labels table
 * @param memory the memory structure
 */
static void labels_table_insert_labels_to_memory(avl_node_t *root, memory_t *memory)
{
    label_t *label;
    linked_list_node_t* lines;
    uint16_t location;
    word_ending_e ending;

    if (!root)
        return;

    label = avl_tree_get_data(root);
    lines = label_get_lines(label);

    ending = (label_get_attributes(label) & EXTERN_FLAG) ? E : R;
    while (lines)
    {
        location = *((int*)linked_list_get_info(lines));
        asm_memory_rewrite_code(memory, label_get_base_address(label), ending, location);
        asm_memory_rewrite_code(memory, label_get_offset(label), ending, location + 1);
        lines = linked_list_get_next_address(lines);
    }

    labels_table_insert_labels_to_memory(avl_tree_get_left_child(root), memory);
    labels_table_insert_labels_to_memory(avl_tree_get_right_child(root), memory);

}

void labels_table_insert_labels_to_memory_proxy(labels_table_t *table, memory_t *memory)
{
    labels_table_insert_labels_to_memory(table->root, memory);
}


/**
 * @brief prints a level of the avl tree
 * 
 * @param root the root of the avl tree
 * @param level the level that is printed
 */
static void labels_table_print_level(avl_node_t *root, int level)
{
    if (!root)
    {
        printf("x  ");
        return;
    }
    if (level == 0)
        printf("%s  ", (label_get_symbol(avl_tree_get_data(root))));
    else
    {
        labels_table_print_level(avl_tree_get_left_child(root), level - 1);
        labels_table_print_level(avl_tree_get_right_child(root), level - 1);
    }
}

void labels_table_print(labels_table_t *table)
{
    int height;
    int level;
    avl_node_t *root;

    root = table->root;
    height = avl_tree_get_height(root);
    for (level = 0; level < height; level++)
    {
        labels_table_print_level(root, level);
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief checks that all of the labels are defined correctly
 * 
 * @param root the root of the labels table
 */
static void labels_table_check_labels_validity(avl_node_t *root)
{
    label_t *label;
    if (!root)
    {
        return;
    }
    label = avl_tree_get_data(root);
    if (label_get_base_address(label) == -1)
    {
        printf("Undefined label \"%s\"\n", label_get_symbol(label));
    }

    labels_table_check_labels_validity(avl_tree_get_left_child(root));
    labels_table_check_labels_validity(avl_tree_get_right_child(root));
}

void labels_table_check_labels_validity_proxy(labels_table_t *table)
{
    labels_table_check_labels_validity(table->root);
}

/**
 * @brief destroys the avl tree containing all the labels structs
 *
 * @param root the root of the avl tree
 */
static void labels_table_destroy_avl_tree(avl_node_t *root)
{
    if (!root)
        return;

    labels_table_destroy_avl_tree(avl_tree_get_left_child(root));
    labels_table_destroy_avl_tree(avl_tree_get_right_child(root));

    label_destroy_lines(avl_tree_get_data(root));
    free(avl_tree_get_data(root));
    free(root);
}

void labels_table_destroy(labels_table_t *table)
{
    labels_table_destroy_avl_tree(table->root);
    free(table);
}
