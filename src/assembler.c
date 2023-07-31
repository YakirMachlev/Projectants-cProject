#include "assembler.h"

#define CHANGE_SUFFIX(file, len, suffix) \
    file[len - 3] = suffix[0];           \
    file[len - 2] = suffix[1];           \
    file[len - 1] = suffix[2];

#define SET_ASSEMBLY_FILE(file_name, file, len) \
    strcpy(file_name, file);                    \
    file_name[len - 4] = '.';                   \
    CHANGE_SUFFIX(file_name, len, "as");

#define SKIP_SPACES(line)      \
    while (isspace(*(line++))) \
        ;                      \
    --line;

#define SKIP_EMPTY_AND_COMMENT(line)   \
    SKIP_SPACES(line)                  \
    if (*line == '\0' || *line == ';') \
    {                                  \
        errors_increase_lines();       \
        continue;                      \
    }
        

/**
 * @brief writes the entire known machine code and data to the memory
 * structure and creates the labels table
 * 
 * @param am_file an assembly file with its macros opened
 * @param labels_table labels_table 
 */
static void assembler_am_iteration(FILE *am_file, labels_table_t *labels_table)
{
    char line[LINE_LENGTH];
    char *line_ptr;

    while (fgets(line, LINE_LENGTH, am_file))
    {
        line_ptr = line;
        SKIP_EMPTY_AND_COMMENT(line_ptr)

        labels_table_add_if_definition(&line_ptr, labels_table); /* adds the label and advances the line pointer to after the label */
        asm_line_analyze(line_ptr);
        errors_increase_lines();
    }
    labels_table_check_labels_validity_proxy(labels_table);
}

/**
 * @brief handles the opening of the .ent and .ext files and writes
 * the labels in the proper format according to the file type
 * 
 * @param file_name the file name
 * @param len the length of the name including the extention
 */
static void assembler_write_entry_and_extern_files(char *file_name, int len)
{
    labels_table_t *table = labels_table_get_instance();
    FILE *entry_file;
    FILE *extern_file;

    /* Entries file */
    CHANGE_SUFFIX(file_name, len, "ent")
    remove(file_name);
    if (labels_table_get_entry_count(table))
    {
        entry_file = fopen(file_name, "w");
    }

    /* Externals file */
    CHANGE_SUFFIX(file_name, len, "ext")
    remove(file_name);
    if (labels_table_get_extern_count(table))
    {
        extern_file = fopen(file_name, "w");
    }

    if (entry_file || extern_file)
    {
        labels_table_write_ext_and_ent_proxy(table, extern_file, entry_file);
        if (entry_file)
        {
            fclose(entry_file);
        }
        if (extern_file)
        {
            fclose(extern_file);
        }
    }
}

void assembler_on_file(char *file)
{
    FILE *as_file;
    FILE *am_file;
    FILE *ob_file;
    FILE *err_file;
    char *file_name;
    int len;
    memory_t *memory;
    labels_table_t *labels_table;

    len = strlen(file) + 4;
    file_name = malloc(len * sizeof(char));
    assert("Memory allocation failed" && file_name != NULL);

    /* Assembly file */
    SET_ASSEMBLY_FILE(file_name, file, len)
    as_file = fopen(file_name, "r");
    assert("File doesn't exist" && as_file);

    /* After macro file */
    CHANGE_SUFFIX(file_name, len, "am")
    am_file = fopen(file_name, "w+");
    macro_write_am_file(as_file, am_file);
    fclose(as_file);

    /* Error file */
    CHANGE_SUFFIX(file_name, len, "err")
    err_file = fopen(file_name, "w");
    /* errors_set_output(err_file); */
    /* fprintf(err_file, "%s", "hello"); */

    memory = asm_memory_get_instance();
    labels_table = labels_table_get_instance();
    fseek(am_file, SEEK_SET, 0);
    assembler_am_iteration(am_file, labels_table);
    fclose(am_file);

    /* Object file */
    CHANGE_SUFFIX(file_name, len, "ob")
    remove(file_name);

    if (ftell(err_file) == 0)
    {
        ob_file = fopen(file_name, "w");
        labels_table_insert_labels_to_memory_proxy(labels_table, memory);
        assembler_write_entry_and_extern_files(file_name, len);

        CHANGE_SUFFIX(file_name, len, "err")
        remove(file_name);

        asm_output_ob_file(ob_file, memory);
    }

    free(file_name);
    fclose(ob_file);
    fclose(err_file);
    asm_memory_destroy(memory);
    labels_table_destroy(labels_table);
}