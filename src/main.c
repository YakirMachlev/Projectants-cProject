#include "assembler.h"

/**
 * @brief converts all of the given assembly files to machine code by
 * analyzing the lines and creating .ob, .ext and .ant files
 * 
 * @param argc the amount of given parameters from terminal
 * @param argv the arguments given in the terminal
 */
void assembler(int argc, char* argv[])
{
    int offset;

    asm_language_init();
    for (offset = 1; offset < argc; offset++)
    {
        assembler_on_file(argv[offset]);
    }    
}

int main(int argc, char* argv[])
{
    assembler(argc, argv);

    return 0;
}