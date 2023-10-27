#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../libraries/Stack/myassert.h"
#include "../enum.h"

struct bytecode_parametrs {
    int                 *buffer;
    size_t               buffer_size;
    size_t               buffer_position;
};

errors_code disassembler(FILE *byte_code_file_pointer, FILE *source_code_file_pointer);
errors_code bytecode_parametrs_constructor(FILE *byte_code_file_pointer, bytecode_parametrs *bytecode_info);
errors_code bytecode_parametrs_destructor(bytecode_parametrs *bytecode_info);
int pop_from_bytecode_buffer(bytecode_parametrs *bytecode_info);
errors_code define_and_print_arg(FILE *source_code_file_pointer, int code_operator, int arg);


#endif //ASSEMBLER_H_INCLUDED
