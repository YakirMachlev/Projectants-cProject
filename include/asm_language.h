#pragma once

#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <inttypes.h>
#include "argument.h"

typedef enum
{
    INVALID_ASM_WORD,
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    JSR,
    RED,
    PRN,
    RTS,
    STOP,
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
} asm_word_e;

/**
 * @brief initialize the assembly language data
 * @file asm_language.h
 */
void asm_language_init();

/**
 * @brief checks if a string is an assembly command
 * @file asm_language.h
 *
 * @param name string to check
 * @return asm_word_e
 */
asm_word_e asm_language_is_instruction(char *name);

/**
 * @brief checks if a string is an assembly register
 * @file asm_language.h
 *
 * @param name string to check
 * @return asm_word_e
 */
asm_word_e asm_language_is_register(char *name);

/**
 * @brief checks if a string is an assembly word
 * @file asm_language.h
 *
 * @param name string to check
 * @return asm_word_e
 */
asm_word_e asm_language_is_saved_word(char *name);

/**
 * @brief gets the opcode of an assembly instruction
 * @file asm_language.h
 *
 * @param instruction assembly instruction
 * @return assembly instruction's opcode
 */
uint8_t asm_language_get_instruction_opcode(asm_word_e instruction);

/**
 * @brief gets the funct of an assembly instruction
 * @file asm_language.h
 *
 * @param instruction assembly instruction
 * @return assembly instruction's funct
 */
uint8_t asm_language_get_instruction_funct(asm_word_e instruction);

/**
 * @brief checks if a source address is valid
 * @file asm_language.h
 *
 * @param instruction assembly instruction
 * @param address source address
 * @return true if valid, else false
 */
bool asm_language_is_valid_src_address(asm_word_e instruction, address_e address);

/**
 * @brief checks if a destination address is valid
 * @file asm_language.h
 *
 * @param instruction assembly instruction
 * @param address destination address
 * @return true if valid, else false
 */
bool asm_language_is_valid_dest_address(asm_word_e instruction, address_e address);

/**
 * @brief gets the num of an assembly register
 * @file asm_language.h
 *
 * @param _register assembly register
 * @return assembly register's num
 */
uint8_t asm_language_get_register_num(asm_word_e _register);

/**
 * @brief gets the number of arguments of an assembly instruction
 * @file asm_language.h
 *
 * @param instruction assembly instruction
 * @return assembly instruction's number of arguments
 */
uint8_t asm_language_get_instruction_args_num(asm_word_e instruction);