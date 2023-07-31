#pragma once

typedef enum {
    IMMEDIATE,
    DIRECT,
    INDEX,
    REGISTER_DIRECT
} address_e;

#include "asm_language.h"
#include "errors.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <inttypes.h>

/**
 * @brief gets the type of address of the argument
 * @file argument.h
 * 
 * @param argument an argument from the assembly file
 * @return address_e 
 */
address_e argument_get_address(char *argument);

/**
 * @brief checks if an argument is valid by his address type
 * @file argument.h
 * 
 * @param argument an argument
 * @param address an address
 * @return true if valid, else false
 */
bool argument_is_valid(char *argument, address_e address);

/**
 * @brief returns the number of words each address method requires
 * @file argument.h
 * 
 * @param address the address method
 * @return uint8_t the amount of  words required
 */
uint8_t argument_get_words_by_address(address_e address);