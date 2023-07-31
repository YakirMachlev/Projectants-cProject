#include "argument.h"

#define CHECK_IMMEDIATE_VALIDATION(argument, is_valid)   \
    is_valid = *argument++ == '#' &&                     \
               (isdigit(*argument) || *argument == '-'); \
    if (is_valid)                                        \
    {                                                    \
        while (isdigit(*(++argument)))                   \
            ;                                            \
        is_valid = *argument == '\0';                    \
    }

#define CHECK_DIRECT_VALIDATION(argument, is_valid)     \
    is_valid = !asm_language_is_saved_word(argument)    \
                && isalpha(*argument);

#define CHECK_INDEX_VALIDATION(argument, is_valid, argument_p, len)     \
    len = strlen(argument);                                             \
    if ((is_valid = argument[len - 1] == ']'))                          \
    {                                                                   \
        argument[len - 1] = '\0';                                       \
        argument_p = strchr(argument, '[');                             \
        if ((is_valid = argument_p != NULL))                            \
        {                                                               \
            *argument_p = '\0';                                         \
            is_valid = asm_language_is_register(argument_p + 1)         \
                && isalpha(*argument)                                   \
                && !asm_language_is_saved_word(argument);               \
            *argument_p = '[';                                          \
        }                                                               \
        argument[len - 1] = ']';                                        \
    }

#define CHECK_REGISTER_DIRECT_VALIDATION(argument, is_valid)    \
    is_valid = true;                                            \
    asm_language_is_register(argument);


address_e argument_get_address(char *argument)
{
    address_e address;
    int len;

    if (*argument == '#')
    {
        address = IMMEDIATE;
    }
    else if (asm_language_is_register(argument))
    {
        address = REGISTER_DIRECT;
    }
    else
    {
        len = strlen(argument);
        if (argument[len - 1] == ']' && *argument != '[' && strchr(argument, '[') != NULL)
        {
            address = INDEX;
        }
        else
        {
            address = DIRECT;
        }
    }

    return address;
}

uint8_t argument_get_words_by_address(address_e address)
{
    uint8_t words;
    assert("Address not valid" && (address >= 0 && address < 4));

    switch (address)
    {
    case IMMEDIATE:
        words = 1;
        break;

    case DIRECT:
    case INDEX:
        words = 2;
        break;

    default:
        words = 0;
    }

    return words;
}

bool argument_is_valid(char *argument, address_e address)
{
    bool is_valid = false;
    char *argument_p;
    int len;

    assert("Address not valid" && (address >= 0 && address < 4));

    switch (address)
    {
    case IMMEDIATE:
        CHECK_IMMEDIATE_VALIDATION(argument, is_valid)
        break;

    case DIRECT:
        CHECK_DIRECT_VALIDATION(argument, is_valid)
        break;

    case INDEX:
        CHECK_INDEX_VALIDATION(argument, is_valid, argument_p, len)
        break;

    case REGISTER_DIRECT:
        CHECK_REGISTER_DIRECT_VALIDATION(argument, is_valid)
        break;

    default:
        is_valid = false;
    }

    return is_valid;
}