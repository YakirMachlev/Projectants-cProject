#include "asm_memory.h"

#define WORD_START 0xFFFF
#define WORD_END 0xF0000
#define WORD_START_BITS 16
#define WORD_END_BITS 4
#define WORD1_ENDING 0x0F
#define WORD2_ENDING 0xF0
#define MEMORY_SIZE 8192
#define MEMORY_WORD_SIZE 20

/* each memory cell is two memory words */
typedef struct memory_cell_t
{
    uint16_t word1;        /* first 16-bits of the first word */
    uint16_t word2;        /* first 16-bits of the second word */
    uint8_t words_ending;   /* last 4-bits of both words */
} memory_cell_t;

struct memory_t
{
    memory_cell_t *memory;
    uint16_t ic;
    uint16_t dc;
};

static memory_t *memory_instance;

/**
 * @brief initializes an asm memory
 * 
 * @return pointer to the initialized memory
 */
static memory_t* asm_memory_init()
{
    memory_t *new_memory;

    new_memory = (memory_t*)malloc(sizeof(memory_t));
    assert("Memory allocation failed (literally)" && new_memory != NULL);

    new_memory->memory = (memory_cell_t*)calloc((MEMORY_SIZE >> 1), sizeof(memory_cell_t));
    assert("Memory allocation failed (literally)" && new_memory->memory != NULL);

    new_memory->ic = START_IC_VALUE;
    new_memory->dc = MEMORY_SIZE - 1;
    return new_memory;
}

memory_t* asm_memory_get_instance()
{
    if (memory_instance == NULL)
    {
        memory_instance = asm_memory_init();
    }
    return memory_instance;
}

uint16_t asm_memory_get_ic(memory_t* asm_memory)
{
    return asm_memory->ic - START_IC_VALUE;
}

uint16_t asm_memory_get_dc(memory_t* asm_memory)
{
    return MEMORY_SIZE - 1 - asm_memory->dc;
}

uint16_t asm_memory_get_pc(memory_t* asm_memory)
{
    return asm_memory->ic - START_IC_VALUE + MEMORY_SIZE - 1 - asm_memory->dc;
}

/**
 * @brief writes a 20-bit word into the memory
 * @file asm_memory.h
 * 
 * @param asm_memory asm memory
 * @param word a 16-bit word
 * @param ending a 4-bit word ending
 * @param location the word's location in the memory
 */
static void asm_memory_write_word(memory_t* asm_memory, uint16_t word, word_ending_e ending, uint16_t location)
{
    memory_cell_t *cell;
    uint16_t index = location >> 1;

    assert("Memory overflow" && asm_memory->ic < asm_memory->dc);
    assert("Invalid memory access" && location < MEMORY_SIZE);

    cell = &(asm_memory->memory[index]);
    if ((location & 1) == 0)
    {
        cell->word1 = word;
        cell->words_ending &= WORD2_ENDING;
        cell->words_ending |= (ending & WORD1_ENDING);
    }
    else
    {
        cell->word2 = word;
        cell->words_ending &= WORD1_ENDING;
        cell->words_ending |= ((ending << 4) & WORD2_ENDING);
    }
}

void asm_memory_push_code(memory_t* asm_memory, uint16_t code_word)
{
    asm_memory_write_word(asm_memory, code_word, A, asm_memory->ic);
    ++(asm_memory->ic);
}

void asm_memory_push_data(memory_t* asm_memory, uint16_t data_word)
{
    asm_memory_write_word(asm_memory, data_word, A, asm_memory->dc);
    --(asm_memory->dc);
}

/**
 * @brief gets a 20-bit word from the memory
 * @file asm_memory.h
 * 
 * @param asm_memory asm memory
 * @param location the word's location in the memory
 * @return uint32_t a 20-bit word
 */
static uint32_t asm_memory_get_word(memory_t* asm_memory, uint16_t location)
{
    uint16_t index = location >> 1;
    uint32_t word;
    memory_cell_t cell;

    assert("Invalid memory access" && location < MEMORY_SIZE);
    
    cell = asm_memory->memory[index];
    if ((location & 1) == 0)
    {
        word = cell.word1 | ((cell.words_ending & WORD1_ENDING) << WORD_START_BITS);
    }
    else
    {
        word = cell.word2 | ((cell.words_ending & WORD2_ENDING) << (WORD_START_BITS - 4));
    }

    return word;
}

uint32_t asm_memory_get_code(memory_t* asm_memory, uint16_t location)
{
    uint32_t word = 0;
    location += START_IC_VALUE;
    if (location <  asm_memory->ic)
    {
        word = asm_memory_get_word(asm_memory, location);
    }
    return word;
}

uint32_t asm_memory_get_data(memory_t* asm_memory, uint16_t location)
{
    uint32_t word = 0;
    location = (MEMORY_SIZE - 1) - location;
    if (location > asm_memory->dc && location < MEMORY_SIZE)
    {
        word = asm_memory_get_word(asm_memory, location);
    }
    return word;
}

void asm_memory_rewrite_code(memory_t* asm_memory, uint16_t code_word, word_ending_e ending, uint16_t location)
{
    location += START_IC_VALUE;
    assert("Invalid memory access" && location <  asm_memory->ic);
    asm_memory_write_word(asm_memory, code_word, ending, location);
}

void asm_memory_destroy(memory_t* asm_memory)
{
    free(asm_memory->memory);
    free(asm_memory);
}
