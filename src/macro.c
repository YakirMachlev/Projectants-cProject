#include "macro.h"

/**
 * @brief checks if a macro is defined in the given line and
 * moves the line pointer to after the "macro" definition (if
 * a macro definition is present)
 * 
 * @param line the line that is being checked
 * @return true if the line contains a macro definition and false elsewise
 */
static bool macro_is_definition(char **line)
{
    bool definition;
    char word[LINE_LENGTH];
    int word_length;
    
    sscanf(*line, "%s%n", word, &word_length);
    definition = strcmp(word, "macro") == 0;
    if (definition)
    {
        *line = *line + word_length;
    }

    return definition;
}

/**
 * @brief checks if the given line is the end of a macro definition
 * 
 * @param line the line that is being checked
 * @return true if the line contains an end of macro definition
 * and false elsewise
 */
static bool macro_is_end_of_macro(char *line)
{
    char word[LINE_LENGTH];
    sscanf(line, "%s", word);
    return strcmp(word, "endm") == 0;
}

/**
 * @brief inserts a macro node to the avl tree
 * 
 * @param root the root of the macros avl tree
 * @param macro_hash the hash of the macro name
 * @return macro_t the created macro node
 */
static macro_t macro_insert_node(macro_t *root, unsigned long macro_hash)
{
    avl_node_t *macro_node;
    macro_node = avl_tree_create_node(macro_hash);
    avl_tree_insert_node(root, macro_node);

    return macro_node;
}

/**
 * @brief allocates a line to be used in the linked list that contains
 * the lines of the macro
 * 
 * @return char* the allocated line
 */
static char* macro_create_content_line()
{
    char* macro_content;
    macro_content = (char*)malloc(sizeof(char) * LINE_LENGTH);
	assert("Couldn't allocate new line for the macro\n" && macro_content);

    return macro_content;
}

/**
 * @brief adds a macro to the macros table (avl tree)
 * 
 * @param fp the file that contains the macro's content
 * @param line the line in which the macro is defined
 * @param root the root of the macros table
 */
static void macro_add_to_table(FILE *fp, char *line, macro_t *root)
{
    char macro_name[LINE_LENGTH];
    macro_t added_macro;
    linked_list_node_t *macro_linked_list_manager;
    char* macro_content;

    macro_linked_list_manager = NULL;
    sscanf(line, "%s", macro_name);
    added_macro = macro_insert_node(root, hash(macro_name));

    macro_content = macro_create_content_line();
    fgets(macro_content, LINE_LENGTH, fp);

    linked_list_push(&macro_linked_list_manager);
    linked_list_set_info(macro_linked_list_manager, macro_content);
    avl_tree_set_data(added_macro, macro_linked_list_manager);

    macro_content = macro_create_content_line();
    while (fgets(macro_content, LINE_LENGTH, fp) && !macro_is_end_of_macro(macro_content))
    {
        linked_list_insert_after_node(macro_linked_list_manager);
        macro_linked_list_manager = linked_list_get_next_address(macro_linked_list_manager);
        linked_list_set_info(macro_linked_list_manager, macro_content);
        macro_content = macro_create_content_line();
    }
}

/**
 * @brief checks a line to see if the line's content is a macro call
 * (searches the macros table). if so, it returns the macro node
 * 
 * @param line the line that is suspected to contain a macro call
 * @param root the root of the macros table
 * @return macro_t the macro node
 */
static macro_t macro_is_called(char* line, macro_t root)
{
    char word[LINE_LENGTH];
    sscanf(line, "%s", word);
    return avl_tree_search_node(root, hash(word));
}

/**
 * @brief opens the given macro's content into the given file
 * 
 * @param output the file in which the macro is opened
 * @param relevant_macro the macro that its content is being copied to the file
 */
static void macro_open_content(FILE *output, macro_t relevant_macro)
{
    linked_list_node_t *macro_linked_list_node;

    macro_linked_list_node = avl_tree_get_data(relevant_macro);
    while (macro_linked_list_node)
    {
        fputs(linked_list_get_info(macro_linked_list_node), output);
        macro_linked_list_node = linked_list_get_next_address(macro_linked_list_node);
    }
}

/**
 * @brief destroys the macro's data (linked list containing the content
 * of the macro)
 * 
 * @param manager the manager of the linked list
 */
static void macro_destroy_linked_list(linked_list_node_t **manager)
{
	linked_list_node_t **start;
	start = manager;

	while (*start)
	{
        free(linked_list_get_info(*start));
		linked_list_pop(start);
	}
}

/**
 * @brief destroys the entire macros table (avl tree)
 * 
 * @param root the root of the avl tree
 */
static void macro_destroy_avl_tree(macro_t root)
{
    linked_list_node_t* list;

    if (!root)
        return;

    macro_destroy_avl_tree(avl_tree_get_left_child(root));
    macro_destroy_avl_tree(avl_tree_get_right_child(root));

    list = avl_tree_get_data(root);

    macro_destroy_linked_list(&list);
    free(root);
}

void macro_write_am_file(FILE* current_file, FILE *am_file)
{
    char line[LINE_LENGTH];
    char *line_ptr;
    macro_t root;
    macro_t relevant_macro;

    root = NULL;
    while (fgets(line, LINE_LENGTH, current_file))
    {
        line_ptr = line;
        if (macro_is_definition(&line_ptr))
        {
            macro_add_to_table(current_file, line_ptr, &root);
        }
        else if ((relevant_macro = macro_is_called(line, root)) != NULL)
        {
            macro_open_content(am_file, relevant_macro);
        }
        else
        {
            fputs(line, am_file);
        }
    }

    macro_destroy_avl_tree(root);
}