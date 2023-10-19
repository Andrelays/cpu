#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../operations.h"
#include "../libraries/Stack/stack.h"
#include <stdio.h>

extern FILE *Global_input_pointer;
extern FILE *Global_output_pointer;

ssize_t disassembler (struct text_parametrs *byte_code);
// int *save_data_to_array(FILE *file_pointer);

#endif //ASSEMBLER_H_INCLUDED
