#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <stdio.h>
#include "enum.h"
#include "onegin.h"
#include "myassert.h"

const ssize_t MAX_SIZE_LABELS =     100;
const ssize_t SIZE_ARRAY_LABELS =   50;
const ssize_t INITIAL_BUFFER_SIZE = 100;

struct label_parametrs {
    char                 label_name[MAX_SIZE_LABELS];
    ssize_t              label_position;
};

struct assem_parametrs {
    int                        *bytecode_buffer;
    label_parametrs             labels_array[SIZE_ARRAY_LABELS];
    ssize_t                     labels_index_position;
    ssize_t                     buffer_size;
    ssize_t                     buffer_position;
    text_parametrs             *source_code;
};

errors_code assembler (assem_parametrs *assem, FILE *byte_code_file_pointer, FILE *listing_file_pointer);

errors_code put_command_in_buffer(const char *string, assem_parametrs *assem, size_t line_number, FILE *listing_file_pointer);
command check_command       (const char *string, size_t *number_args);
bool check_command_args  (const char *string_without_command, size_t number_args, short *command_id, double *number, char *reg, assem_parametrs *assem, ssize_t arg_len);
bool check_is_register   (const char *string_without_command, char *reg,   ssize_t arg_len);
bool check_is_number(const char *string_without_command, short command_id, double *number, ssize_t arg_len);
int strncmp_case_insensitive(const char *str_1, const char *str_2, size_t number_comparisons);

errors_code assem_parametrs_constructor(assem_parametrs *assem, FILE *source_code_file_pointer);
errors_code replace_newline_char_and_comments(char *text);

errors_code push_in_bytecode_buffer(assem_parametrs *assem, int number);
errors_code check_size_buffer(assem_parametrs *assem);

ssize_t find_label(char *label, assem_parametrs *assem);
bool check_is_label(const char *string, assem_parametrs *assem);

errors_code assem_parametrs_destructor(assem_parametrs *assem);

errors_code output_to_listing_file(const char *string, size_t number_args, size_t line_number, FILE *listing_file_pointer, assem_parametrs *assem);
errors_code putn(char symbol, size_t number_symbols, FILE *file);
errors_code translate_var_to_binary_and_print(int number, FILE *file);

const char *check_is_empty_string(const char *string);

const char *parsing_string_to_tokens(const char *string, ssize_t *token_length, const char *separator);
ssize_t find_token_size(const char *token, const char *separator);

#endif //ASSEMBLER_H_INCLUDED
