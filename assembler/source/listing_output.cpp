#include <climits>
#include "assembler.h"
#include "colors.h"

FILE *Global_logs_pointer  = stderr;
bool  Global_color_output  = false;

errors_code output_to_listing_file(const char *string, size_t number_args, size_t line_number, FILE *listing_file_pointer, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(listing_file_pointer       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    Global_logs_pointer = listing_file_pointer;

    COLOR_PRINT(LightGray, "%-4lu: ", line_number);

    COLOR_PRINT(MediumBlue, "%p ", assem->bytecode_buffer + assem->buffer_position - number_args - 1);

    translate_var_to_binary_and_print(*(assem->bytecode_buffer + assem->buffer_position - number_args - 1), listing_file_pointer);

    COLOR_PRINT(Green, "%-40s ", string);

    for (ssize_t index_position = assem->buffer_position - (ssize_t) number_args; index_position < assem->buffer_position; index_position++)
        COLOR_PRINT(BlueViolet, "|%d|", assem->bytecode_buffer[index_position]);

    fprintf(Global_logs_pointer, "\n");

    return ASSERT_NO_ERROR;
}

errors_code putn(char symbol, size_t number_symbols, FILE *file)
{
    MYASSERT(file != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    Global_logs_pointer = file;

    for (size_t counter_symbols = 0; counter_symbols < number_symbols; counter_symbols++)
        putc(symbol, file);

    putc('\n', Global_logs_pointer);

    return ASSERT_NO_ERROR;
}

errors_code translate_var_to_binary_and_print(int number, FILE *file)
{
    MYASSERT(file != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    Global_logs_pointer = file;

    char binary[CHAR_BIT + 2] = "";

    int checked_bit = 1;

    for (ssize_t index_bit = CHAR_BIT; index_bit > -1; index_bit--)
    {
        if (index_bit == 3) {
            continue;
        }

        if (number & checked_bit) {
            binary[index_bit] = '1';
        }

        else {
            binary[index_bit] = '0';
        }

        checked_bit = checked_bit << 1;
    }


    COLOR_PRINT(Maroon, "<%s\'", binary);

    COLOR_PRINT(RED, "%s> ", binary + 4);

    return ASSERT_NO_ERROR;
}

