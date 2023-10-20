#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "../enum.h"
#include "../libraries/Onegin/onegin.h"
#include <stdio.h>

const int SIZE_ARRAY_LABELS = 10;

struct assem_parametrs {
    int                 *bytecode_buffer;
    int                  labels[SIZE_ARRAY_LABELS];
    const ssize_t        labels_size = SIZE_ARRAY_LABELS;
    ssize_t              buffer_size;
    ssize_t              buffer_position;
};

ssize_t assembler (assem_parametrs *assem, FILE *source_code_pointer, FILE *byte_code_pointer, FILE *listening_file_pointer);

void replace_newline_char_and_comments(char *text);

ssize_t put_command_id_in_buffer(const char *string, assem_parametrs *assem, FILE *listening_file_pointer);

void push_in_bytecode_buffer(assem_parametrs *assem, int number);

void check_size_buffer(assem_parametrs *assem);

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg, assem_parametrs *assem);
bool check_is_empty_string(const char *string);
bool check_is_label(const char *string, assem_parametrs *assem);

ssize_t assem_parametrs_constructor(assem_parametrs *assem);

ssize_t assem_parametrs_destructor(assem_parametrs *assem);

void output_to_listening_file(const char *string, size_t number_args, FILE *listening_file_pointer, assem_parametrs *assem);

#endif //ASSEMBLER_H_INCLUDED
