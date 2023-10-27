#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"

const size_t NUMBER_SEPARATORS = 80;

errors_code assembler(assem_parametrs *assem, FILE *byte_code_file_pointer, FILE *listing_file_pointer)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(listing_file_pointer       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(byte_code_file_pointer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    text_parametrs *source_code = assem->source_code;

    rewind(byte_code_file_pointer);
    assem->buffer_position = 0;

    for (size_t position = 0; position < source_code->number_lines; position++)
    {
        char *string = ((source_code->string_array)[position]).string_pointer;

        if (check_is_empty_string(string) || check_is_label(string, assem))
            continue;

        if (put_command_in_buffer(string, assem, position  + 1, listing_file_pointer) == INVALID_OPERATOR)
            return INVALID_OPERATOR;
    }

    fwrite(assem->bytecode_buffer, sizeof(int), (size_t) assem->buffer_position, byte_code_file_pointer);

    putn('-', NUMBER_SEPARATORS, listing_file_pointer);

    return ASSERT_NO_ERROR;
}

bool check_is_empty_string(const char *string)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    for(size_t string_index = 0; string[string_index] != '\0'; string_index++)
    {
        if (!isspace(string[string_index]) && string[string_index] != '\0') {
            return false;
        }
    }

    return true;
}
