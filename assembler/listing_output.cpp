#include <climits>
#include "assembler.h"

errors_code output_to_listing_file(const char *string, size_t number_args, FILE *listing_file_pointer, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(listing_file_pointer       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    fprintf(listing_file_pointer, "%p ", assem->bytecode_buffer + assem->buffer_position - number_args - 1);

    translate_var_to_binary_and_print(*(assem->bytecode_buffer + assem->buffer_position - number_args - 1), listing_file_pointer);

    fprintf(listing_file_pointer, "%-30s |", string);

    for (ssize_t index_position = assem->buffer_position - (ssize_t) number_args; index_position < assem->buffer_position; index_position++)
        fprintf(listing_file_pointer, "%d|", assem->bytecode_buffer[index_position]);

    fprintf(listing_file_pointer, "\n");

    return ASSERT_NO_ERROR;
}

errors_code putn(char symbol, size_t number_symbols, FILE *file)
{
    MYASSERT(file != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t counter_symbols = 0; counter_symbols < number_symbols; counter_symbols++)
        putc(symbol, file);

    putc('\n', file);

    return ASSERT_NO_ERROR;
}

errors_code translate_var_to_binary_and_print(int number, FILE *file)
{
    MYASSERT(file != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    char binary[CHAR_BIT + 1] = "";
    int checked_bit = 1;

    for (ssize_t index_bit = CHAR_BIT - 1; index_bit > -1; index_bit--)
    {
        if (number & checked_bit)
            binary[index_bit] = '1';

        else
            binary[index_bit] = '0';

        checked_bit = checked_bit << 1;
    }

    fprintf(file, "<%s> ", binary);

    return ASSERT_NO_ERROR;
}

