#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "../libraries/Onegin/onegin.h"
#include "../libraries/Onegin/myassert.h"


const ssize_t MAX_SIZE_LABELS =     100;
const ssize_t SIZE_ARRAY_LABELS =   50;
const ssize_t INITIAL_BUFFER_SIZE = 100;

struct label_parametrs {
    char                 label_name[MAX_SIZE_LABELS];
    ssize_t              label_position;
};

struct assem_parametrs {
    int                 *bytecode_buffer;
    label_parametrs      labels_array[SIZE_ARRAY_LABELS];
    ssize_t              labels_index_position;
    ssize_t              buffer_size;
    ssize_t              buffer_position;
};

errors_code assembler (assem_parametrs *assem, FILE *source_code_pointer, FILE *byte_code_pointer, FILE *listening_file_pointer);

errors_code put_command_id_in_buffer(const char *string, assem_parametrs *assem, FILE *listening_file_pointer);

errors_code replace_newline_char_and_comments(char *text);

errors_code push_in_bytecode_buffer(assem_parametrs *assem, int number);

errors_code check_size_buffer(assem_parametrs *assem);

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg, assem_parametrs *assem);
bool check_is_empty_string(const char *string);
bool check_is_label(const char *string, assem_parametrs *assem);

errors_code assem_parametrs_constructor(assem_parametrs *assem);

errors_code assem_parametrs_destructor(assem_parametrs *assem);

errors_code output_to_listening_file(const char *string, size_t number_args, FILE *listening_file_pointer, assem_parametrs *assem);

ssize_t find_label(char *label, assem_parametrs *assem);

#endif //ASSEMBLER_H_INCLUDED
