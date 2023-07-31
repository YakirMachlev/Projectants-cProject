#include "asm_output.h"

/**
 * @brief writes the binary memory words in the hexa format
 * 
 * @param ob_file object file
 * @param word binary 20-bit word
 */
static void asm_output_write_in_hexa_format(FILE *ob_file, uint32_t word)
{
    char hexa[5];
    uint8_t offset;

    hexa[0] = (word & 0xF0000) >> 16;
    hexa[1] = (word & 0x0F000) >> 12;
    hexa[2] = (word & 0x00F00) >> 8;
    hexa[3] = (word & 0x000F0) >> 4;
    hexa[4] = (word & 0x0000F); 

    fprintf(ob_file, "%c%x", 'A', hexa[0]);
    for (offset = 1; offset < 5; ++offset)
    {
        fprintf(ob_file, "-%c%x", 'A' + offset, hexa[offset]);
    }
}

void asm_output_extern_label(FILE *ext_file, label_t *label)
{
    char* symbol;
    linked_list_node_t *lines;
    int line;
    unsigned int offset;
    
    lines = label_get_lines(label);
    symbol = label_get_symbol(label);

    while (lines)
    {
        line = *((int*)linked_list_get_info(lines)) + START_IC_VALUE;
        offset = line % 16;
        fprintf(ext_file, "%s BASE %d\n", symbol, line - offset);
        fprintf(ext_file, "%s OFFSET %d\n\n", symbol, offset);
        lines = linked_list_get_next_address(lines);
    }
}

void asm_output_entry_label(FILE *ent_file, label_t *label)
{
    fprintf(ent_file, "%s,%d,%d\n", label_get_symbol(label), label_get_base_address(label), label_get_offset(label));
}

void asm_output_ob_file(FILE *ob_file, memory_t *memory)
{
    uint32_t word;
    uint16_t line = START_IC_VALUE;
    uint16_t icf;
    uint16_t dcf;
    uint16_t location;

    icf = asm_memory_get_ic(memory);
    dcf = asm_memory_get_dc(memory);

    fprintf(ob_file, "%4d\t%4d\n", icf, dcf);

    for (location = 0; location < icf; ++location)
    {
        word = asm_memory_get_code(memory, location);
        fprintf(ob_file, "%04d\t", line++);
        asm_output_write_in_hexa_format(ob_file, word);
        fprintf(ob_file, "%c", '\n');
    }

    for (location = 0; location < dcf; ++location)
    {
        word = asm_memory_get_data(memory, location);
        fprintf(ob_file, "%04d\t", line++);
        asm_output_write_in_hexa_format(ob_file, word);
        fprintf(ob_file, "%c", '\n');
    }
}