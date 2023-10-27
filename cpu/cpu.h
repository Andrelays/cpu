#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../enum.h"
#include "../libraries/Stack/stack.h"
#include "../libraries/Onegin/myassert.h"
#include <stdio.h>

const ssize_t MEMORY_SIZE = 100;

struct bytecode_parametrs {
    int                 *buffer;
    size_t               buffer_size;
    size_t               buffer_position;
};

errors_code processor(FILE *byte_code_pointer, FILE *logs_pointer);
errors_code bytecode_parametrs_constructor(FILE *byte_code_pointer, bytecode_parametrs *bytecode_info);
errors_code bytecode_parametrs_destructor(bytecode_parametrs *bytecode_info);
int pop_from_bytecode_buffer(bytecode_parametrs *bytecode_info);

#endif //ASSEMBLER_H_INCLUDED
