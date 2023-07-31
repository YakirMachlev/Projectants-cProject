#include "asm_language.h"

#define NUMBER_OF_INSTRUCTIONS 16
#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_ASM_WORDS 32
#define NUMBER_OF_LETTERS 26

#define ALPHA_INDEX(ch) ((unsigned char)((ch) - 'a') % NUMBER_OF_LETTERS)

#define INIT_INSTRUCTION(_name, _opcode, _funct, _address_src, _address_dest, key) \
    key = asm_language_hash(_name);                                                \
    strcpy(asm_data[key].name, _name);                                             \
    asm_data[key].opcode = _opcode;                                                \
    asm_data[key].funct = _funct;                                                  \
    asm_data[key].address_src = _address_src;                                      \
    asm_data[key].address_dest = _address_dest;

#define INIT_REGISTER(_name, _opcode, key) \
    INIT_INSTRUCTION(_name, _opcode, 0, 0, 0, key)

#define CHECK_NAME_VALIDATION(name, key, is_valid, condition)   \
    is_valid = false;                                           \
    if (name)                                                   \
    {                                                           \
        key = asm_language_hash(name);                          \
        if (condition)                                          \
        {                                                       \
            is_valid = (strcmp(asm_data[key].name, name) == 0); \
        }                                                       \
    }

#define ASSERT_VALID_ASM_WORD(word) \
    assert("Invalid asm word" && (word > INVALID_ASM_WORD && word <= NUMBER_OF_ASM_WORDS))

#define ASSERT_VALID_ASM_INSTRUCTION(word) \
    assert("Invalid asm instruction" && (word > INVALID_ASM_WORD && word <= NUMBER_OF_INSTRUCTIONS))

#define ASSERT_VALID_ASM_REGISTER(word) \
    assert("Invalid asm register" && (word >= NUMBER_OF_INSTRUCTIONS && word <= NUMBER_OF_ASM_WORDS))

typedef struct
{
    char name[5];                  /* name */
    unsigned int opcode : 4;       /* opcode 0-15 */
    unsigned int funct : 4;        /* funct 10-13 */
    unsigned int address_src : 4;  /* source address by bits */
    unsigned int address_dest : 4; /* destination address by bits */
} data_t;

/*
 * instructions 0 - 15
 * registers 15 - 31
 */
static data_t asm_data[NUMBER_OF_ASM_WORDS + 1];
static unsigned int asm_language_hash(const char *str);

void asm_language_init()
{
    unsigned int key;

    /**
     * 1|2|4|8 - address bits:
     * 1 - IMMEDIATE
     * 2 - DIRECT
     * 4 - INDEX
     * 8 - REGISTER_DIRECT
     */
    INIT_INSTRUCTION("mov", 0, 0, 1 | 2 | 4 | 8, 2 | 4 | 8, key)
    INIT_INSTRUCTION("cmp", 1, 0, 1 | 2 | 4 | 8, 1 | 2 | 4 | 8, key)
    INIT_INSTRUCTION("add", 2, 10, 1 | 2 | 4 | 8, 2 | 4 | 8, key)
    INIT_INSTRUCTION("sub", 2, 11, 1 | 2 | 4 | 8, 2 | 4 | 8, key)
    INIT_INSTRUCTION("lea", 4, 0, 2 | 4, 2 | 4 | 8, key)
    INIT_INSTRUCTION("clr", 5, 10, 0, 2 | 4 | 8, key)
    INIT_INSTRUCTION("not", 5, 11, 0, 2 | 4 | 8, key)
    INIT_INSTRUCTION("inc", 5, 12, 0, 2 | 4 | 8, key)
    INIT_INSTRUCTION("dec", 5, 13, 0, 2 | 4 | 8, key)
    INIT_INSTRUCTION("jmp", 9, 10, 0, 2 | 4, key)
    INIT_INSTRUCTION("bne", 9, 11, 0, 2 | 4, key)
    INIT_INSTRUCTION("jsr", 9, 12, 0, 2 | 4, key)
    INIT_INSTRUCTION("red", 12, 0, 0, 2 | 4 | 8, key)
    INIT_INSTRUCTION("prn", 13, 0, 0, 1 | 2 | 4 | 8, key)
    INIT_INSTRUCTION("rts", 14, 0, 0, 0, key)
    INIT_INSTRUCTION("stop", 15, 0, 0, 0, key)

    INIT_REGISTER("r0", 0, key)
    INIT_REGISTER("r1", 1, key)
    INIT_REGISTER("r2", 2, key)
    INIT_REGISTER("r3", 3, key)
    INIT_REGISTER("r4", 4, key)
    INIT_REGISTER("r5", 5, key)
    INIT_REGISTER("r6", 6, key)
    INIT_REGISTER("r7", 7, key)
    INIT_REGISTER("r8", 8, key)
    INIT_REGISTER("r9", 9, key)
    INIT_REGISTER("r10", 10, key)
    INIT_REGISTER("r11", 11, key)
    INIT_REGISTER("r12", 12, key)
    INIT_REGISTER("r13", 13, key)
    INIT_REGISTER("r14", 14, key)
    INIT_REGISTER("r15", 15, key)
}

static unsigned int asm_language_hash(const char *str)
{
    static const unsigned char asso_values[] = {
         1, 14,  9,  5,  3, 29, 28, 27, 24, 23, 
        37, 12,  7,  1, 19,  6, 37,  0,  0,  0, 
        11,  0, 37, 37, 37, 12};

    /*
     * instructions 0 - 15
     * registers 15 - 31
     */
    static const signed char lookup[] = {
        15, 18, 28, 22, 32, 21, 31, 14, 13, 20, 
        30,  3, 17, 27, 19, 29,  5,  9, 11, 16, 
         7,  6,  2, 12, 26,  4,  1, 25, 24, 23, 
         0,  0,  0,  0,  8,  0, 10};

    unsigned int len = strlen(str);

    unsigned int hval = 0;

    switch (len)
    {
        default:
            hval += asso_values[ALPHA_INDEX(str[2])];
        case 2:
            hval += asso_values[ALPHA_INDEX(str[1])];
        case 1:
            hval += asso_values[ALPHA_INDEX(str[0])];
            break;
    }

    return hval < 37 ? lookup[hval] : 0;
}

asm_word_e asm_language_is_instruction(char *name)
{
    bool is_valid;
    unsigned int key = 0;
    CHECK_NAME_VALIDATION(name, key, is_valid, key > 0 && key <= NUMBER_OF_INSTRUCTIONS)
    return is_valid ? key : INVALID_ASM_WORD;
}

asm_word_e asm_language_is_register(char *name)
{
    bool is_valid;
    unsigned int key;
    CHECK_NAME_VALIDATION(name, key, is_valid, key > NUMBER_OF_INSTRUCTIONS)
    return is_valid ? key : INVALID_ASM_WORD;
}

asm_word_e asm_language_is_saved_word(char *name)
{
    bool is_valid = false;
    unsigned int key;
    CHECK_NAME_VALIDATION(name, key, is_valid, key > 0)
    return is_valid ? key : INVALID_ASM_WORD;
}

uint8_t asm_language_get_instruction_opcode(asm_word_e instruction)
{
    ASSERT_VALID_ASM_INSTRUCTION(instruction);
    return asm_data[instruction].opcode;
}

uint8_t asm_language_get_instruction_funct(asm_word_e instruction)
{
    ASSERT_VALID_ASM_INSTRUCTION(instruction);
    return asm_data[instruction].funct;
}

bool asm_language_is_valid_src_address(asm_word_e instruction, address_e address)
{
    ASSERT_VALID_ASM_INSTRUCTION(instruction);
    return asm_data[instruction].address_src & (1 << address);
}

bool asm_language_is_valid_dest_address(asm_word_e instruction, address_e address)
{
    ASSERT_VALID_ASM_INSTRUCTION(instruction);
    return asm_data[instruction].address_dest & (1 << address);
}

uint8_t asm_language_get_register_num(asm_word_e _register)
{
    ASSERT_VALID_ASM_REGISTER(_register);
    return asm_data[_register].opcode;
}

uint8_t asm_language_get_instruction_args_num(asm_word_e instruction)
{
    data_t data;
    ASSERT_VALID_ASM_INSTRUCTION(instruction);
    data = asm_data[instruction];
    return !!data.address_src + !!data.address_dest;
}
