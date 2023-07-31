#include "asm_line.h"

#define SKIP_SPACES(line)      \
    while (isspace(*(line++))) \
        ;                      \
    --line;

#define SCAN_ARGUMENT(line, argument, i)                      \
    for (i = 0; *line != ',' && !isspace(*line); ++line, ++i) \
    {                                                         \
        argument[i] = *line;                                  \
    }                                                         \
    argument[i] = '\0';

#define SEARCH_COMMA(line)            \
    while (*line && *(line++) != ',') \
        ;                             \
    --line;

/**
 * @brief gets an asm argument and checks its validity. If valid, adds
 * it to the memory and returns the num of words it wrote to the memory.
 * Else, writes an error to the error file
 * 
 * @param line the line in which the argument is present
 * @param instruction the asm instruction to which the argument belongs
 * @param is_src is the argument src or dest
 * @param reg pointer to a register number
 * @param addr pointer to the argument address method
 * @param memory the memory structure
 * @return uint8_t the number of words it wrote to the memory
 */
static uint8_t asm_line_handle_argument(char *line, asm_word_e instruction, bool is_src, uint8_t *reg, uint8_t *addr, memory_t *memory)
{
    uint8_t i;
    address_e address;
    label_t *label;
    char *opening_bracket;
    char *closing_bracket;
    char argument[LABEL_MAX_LENGTH];
    uint8_t words = 0;

    SCAN_ARGUMENT(line, argument, i)
    address = argument_get_address(argument);

    if (argument_is_valid(argument, address))
    {
        if ((is_src && asm_language_is_valid_src_address(instruction, address))
        || (!is_src && asm_language_is_valid_dest_address(instruction, address)))
        {
            *addr = address;
            words += argument_get_words_by_address(address);

            switch (address)
            {
            case INDEX:
                opening_bracket = strchr(argument, '[');
                *(opening_bracket++) = '\0';
                closing_bracket = strchr(opening_bracket, ']');
                *closing_bracket = '\0';
                *reg = asm_language_get_register_num(asm_language_is_register(opening_bracket));
            case DIRECT:
                label = labels_table_get_forced_label(labels_table_get_instance(), argument);
                label_insert_line(label, asm_memory_get_ic(asm_memory_get_instance()));
                asm_memory_push_code(memory, 0); /* skips lable lines */
                asm_memory_push_code(memory, 0);
                break;
            case REGISTER_DIRECT:
                *reg = asm_language_get_register_num(asm_language_is_register(argument));
                break;
            case IMMEDIATE:
                asm_memory_push_code(memory, atoi(argument + 1));
                break;
            }
        }
        else
        {
            errors_print_line(INVALID_ADDRESS_METHOD);
        }
    }
    else
    {
        errors_print_line(INVALID_ARGUMENT);
    }

    return words;
}

/**
 * @brief converts the entire asm line into a 16-bit memory word format
 * 
 * @param instruction the asm instruction
 * @param reg_src the source register num
 * @param addr_src the source address method
 * @param reg_dest the destination register num
 * @param addr_dest the destination address method
 * @return uint16_t the converted word (without the ending)
 */
static uint16_t asm_line_get_args_word(asm_word_e instruction, uint8_t reg_src, uint8_t addr_src, uint8_t reg_dest, uint8_t addr_dest)
{
    uint16_t word = 0;
    word = addr_dest | (reg_dest << 2) | (addr_src << 6) | (reg_src << 8);
    word |= (asm_language_get_instruction_funct(instruction) << 12);
    return word;
}

/**
 * @brief analyzes an asm command line
 * 
 * @param line the asm line
 */
static void asm_line_analyze_command(char *line)
{
    char instruction_name[6];
    asm_word_e instruction;
    uint8_t args_num;
    uint8_t reg_src = 0, reg_dest = 0;
    uint8_t addr_src = 0, addr_dest = 0;
    memory_t *memory;
    uint8_t words = 1;
    uint16_t args_word;

    SKIP_SPACES(line)
    sscanf(line, "%6s", instruction_name);

    if ((instruction = asm_language_is_instruction(instruction_name)) != INVALID_ASM_WORD)
    {
        memory = asm_memory_get_instance();
        asm_memory_push_code(memory, 1 << asm_language_get_instruction_opcode(instruction));
        line += 4;
        args_num = asm_language_get_instruction_args_num(instruction);
        
        if (args_num == 1)
        {
            asm_memory_push_code(memory, 0); /* skips the args word */
            SKIP_SPACES(line)
            words += asm_line_handle_argument(line, instruction, false, &reg_dest, &addr_dest, memory);
            args_word = asm_line_get_args_word(instruction, reg_src, addr_src, reg_dest, addr_dest);
            asm_memory_rewrite_code(memory, args_word, 4, asm_memory_get_ic(memory) - words);
            while (!isspace(*line)) ++line;
        }
        else if (args_num == 2)
        {
            asm_memory_push_code(memory, 0); /* skips the args word */
            SKIP_SPACES(line)
            words += asm_line_handle_argument(line, instruction, true, &reg_src, &addr_src, memory);
            
            SEARCH_COMMA(line)
            if (*(line++) == ',')
            {
                SKIP_SPACES(line)
                words += asm_line_handle_argument(line, instruction, false, &reg_dest, &addr_dest, memory);
                args_word = asm_line_get_args_word(instruction, reg_src, addr_src, reg_dest, addr_dest);
                asm_memory_rewrite_code(memory, args_word, 4, asm_memory_get_ic(memory) - words);
                while (!isspace(*line)) ++line;
            }
            else
            {
                errors_print_line(MISSING_ARGUMENTS);
            }
            
        }
        
        
        if (sscanf(line, "%1s", &args_num) == 1)
        {
            printf("%s", line);
            errors_print_line(EXTRANEOUS_TEXT);
        }
    }
    else
    {
        errors_print_line(INVALID_INSTRUCTION);
    }
}

void asm_line_analyze(char *line)
{
    char first_char;
    sscanf(line, "%1s", &first_char);

    if (first_char == '.')
    {
        directive_handle(line);
    }
    else
    {
        asm_line_analyze_command(line);
    }
}
