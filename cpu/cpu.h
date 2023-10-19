#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../enum.h"
#include "../libraries/Stack/stack.h"
#include <stdio.h>

struct bytecode_parametrs {
    int                 *buffer;
    ssize_t              buffer_size;
    ssize_t              buffer_position;
};

ssize_t processor(FILE *byte_code_pointer, FILE *logs_pointer);

ssize_t bytecode_parametrs_constructor(FILE *byte_code_pointer, bytecode_parametrs *bytecode_info);

int pop_from_bytecode_buffer(bytecode_parametrs *bytecode_info);

ssize_t bytecode_parametrs_destructor(bytecode_parametrs *bytecode_info);

#endif //ASSEMBLER_H_INCLUDED
