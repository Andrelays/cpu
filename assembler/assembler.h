#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "../processor.h"
#include <stdio.h>

extern FILE *Global_input_pointer;
extern FILE *Global_output_pointer;

ssize_t assembler (struct text_parametrs *source_code);

void replace_newline_char(char *text);

operators find_operator (const char *string);
bool check_is_empty_string(const char *string);
bool check_is_correct_operator(operators code_operator, size_t line_number, const char *string);
bool check_is_correct_number_push(const char *string, size_t line_number);

#endif //ASSEMBLER_H_INCLUDED
