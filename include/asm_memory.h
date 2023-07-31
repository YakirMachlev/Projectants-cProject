#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define START_IC_VALUE 100

typedef enum
{
    A = 4,
    R = 2,
    E = 1
} word_ending_e;

typedef struct memory_t memory_t;

/**
 * @brief gets the asm memory instance
 * @file asm_memory.h
 *
 * @return memory_t* the memory instance
 */
memory_t *asm_memory_get_instance();

/**
 * @brief gets the number of code words pushed
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @return uint16_t instructions counter
 */
uint16_t asm_memory_get_ic(memory_t *asm_memory);

/**
 * @brief gets the number of data words pushed
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @return uint16_t data counter
 */
uint16_t asm_memory_get_dc(memory_t *asm_memory);

/**
 * @brief gets the number of words pushed
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @return uint16_t pushed counter
 */
uint16_t asm_memory_get_pc(memory_t *asm_memory);

/**
 * @brief push a 16-bit code word to the memory
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @param code_word 16-bit code word
 */
void asm_memory_push_code(memory_t *asm_memory, uint16_t code_word);

/**
 * @brief push a 16-bit data word to the memory
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @param data_word 16-bit data word
 */
void asm_memory_push_data(memory_t *asm_memory, uint16_t data_word);

/**
 * @brief gets a 20-bit code word from the memory
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @param location the word's location in the memory
 * @return uint32_t 0 if no code left, else a 20-bit word
 */
uint32_t asm_memory_get_code(memory_t *asm_memory, uint16_t location);

/**
 * @brief gets a 20-bit data word from the memory
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @param location the word's location in the memory
 * @return uint32_t 0 if no code left, else a 20-bit word
 */
uint32_t asm_memory_get_data(memory_t *asm_memory, uint16_t location);

/**
 * @brief rewrites a 20-bit code word
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 * @param word a 16-bit word
 * @param ending a 4-bit word ending
 * @param location the word's location in the memory
 */
void asm_memory_rewrite_code(memory_t *asm_memory, uint16_t code_word, word_ending_e ending, uint16_t location);

/**
 * @brief destroys an asm memory
 * @file asm_memory.h
 *
 * @param asm_memory asm memory
 */
void asm_memory_destroy(memory_t *asm_memory);