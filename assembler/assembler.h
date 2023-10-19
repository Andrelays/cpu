#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "../operations.h"
#include <stdio.h>

struct assem_parametrs {
    int                 *bytecode_buffer;
    ssize_t              buffer_size;
    ssize_t              buffer_position;
};

ssize_t assembler (assem_parametrs *assem, FILE *source_code_pointer, FILE *byte_code_pointer);

void replace_newline_char_and_comments(char *text);

ssize_t put_command_id_in_buffer(const char *string, assem_parametrs *assem);

void push_in_bytecode_buffer(assem_parametrs *assem, int number);

void check_size_buffer(assem_parametrs *assem);

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg);
bool check_is_empty_string(const char *string);

ssize_t assem_parametrs_constructor(assem_parametrs *assem);

ssize_t assem_parametrs_destructor(assem_parametrs *assem);

#endif //ASSEMBLER_H_INCLUDED
