#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

errors_code put_command_in_buffer(const char *string, assem_parametrs *assem, size_t line_number, FILE *listing_file_pointer)
{
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(listing_file_pointer       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    short command_id    = NO_OPERATOR;
    ssize_t arg_len     = 0;
    double number       = 0;
    char reg            = 0;
    size_t number_args  = 0;


    if ((command_id = check_command(string, &number_args)) != NO_OPERATOR)
    {
        const char *string_without_command = parsing_string_to_tokens(string, &arg_len, " \r\n\t");

        if (!check_command_args(string_without_command, number_args, &command_id, &number, &reg, assem, arg_len))
        {
            printf(RED "%lu: ERROR COMMAND: %s\n" RESET_COLOR, line_number, string);
            return INVALID_OPERATOR;
        }

        // > 1 args
        if (string_without_command && parsing_string_to_tokens(string_without_command, NULL, " \r\n\t"))
        {
            printf(RED "%lu: ERROR COMMAND: %s\n" RESET_COLOR, line_number, string);
            return INVALID_OPERATOR;
        }

        push_in_bytecode_buffer(assem, command_id);

        if (number_args > 0 && command_id & COMMAND_ARGS_REGISTER) {
            push_in_bytecode_buffer(assem, reg - 'a' + 1);
        }

        if (number_args > 0 && command_id & COMMAND_ARGS_IMMEDIATE) {
            push_in_bytecode_buffer(assem, (int) number);
        }

        #ifdef DEBUG_LISTING
            output_to_listing_file(string, number_args, line_number, listing_file_pointer, assem);
        #endif

        return ASSERT_NO_ERROR;
    }

    printf(RED "%lu: ERROR COMMAND: %s\n" RESET_COLOR, line_number, string);

    return INVALID_OPERATOR;
}

command check_command(const char *string, size_t *number_args)
{
    #define DEF_COMMAND(command, id, command_number_args, ...)                                      \
        if (strncmp_case_insensitive(string, #command, sizeof #command - 1) == 0) {                 \
            if(string[sizeof #command - 1] == '\0' || isspace(string[sizeof #command - 1]))         \
            {                                                                                       \
                *number_args = command_number_args;                                                 \
                                                                                                    \
                return command;                                                                     \
            }                                                                                       \
        }                                                                                           \

    #include "../commands.h"

    #undef DEF_COMMAND

    return NO_OPERATOR;
}

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, double *number, char *reg, assem_parametrs *assem, ssize_t arg_len)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(command_id                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(number                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(reg                        != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    char label[MAX_SIZE_LABELS] = {};

    if (!string_without_command)
    {
        if (number_args == 0) {
            return true;
        }

        return false;
    }

    if (number_args == 1)
    {
        if (string_without_command[0] == '[' && string_without_command[arg_len - 1] == ']')
        {
            *command_id |= COMMAND_ARGS_MEMORY;
            arg_len -= 2;
            ++string_without_command;
        }

        if (check_is_register(string_without_command, reg, arg_len))
        {
            *command_id |= COMMAND_ARGS_REGISTER;

            return true;
        }

        if(string_without_command[arg_len - 1] == ':')
        {
            strncpy(label, string_without_command, (size_t) arg_len - 1);

            *command_id |= COMMAND_ARGS_IMMEDIATE;
            *number = (double) find_label(label, assem);

            return true;
        }

        if(check_is_number(string_without_command, *command_id, number, arg_len))
        {
            *command_id |= COMMAND_ARGS_IMMEDIATE;

            return true;
        }
    }

    return false;
}

bool check_is_register(const char *string_without_command, char *reg, ssize_t arg_len)
{
    MYASSERT(string_without_command != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(reg                    != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    if (string_without_command[0] == 'r' && string_without_command[2] == 'x' && arg_len == 3)
    {
        *reg = string_without_command[1];

        if(*reg - 'a' >= 0 && *reg - 'a' <= NUMBER_OF_REGISTERS - 1)
            return true;
    }

    return false;
}

bool check_is_number(const char *string_without_command, short command_id, double *number, ssize_t arg_len)
{
    MYASSERT(string_without_command != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(number                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    char *pointer_to_end_of_number = NULL;

    if (command_id & COMMAND_ARGS_MEMORY) {
        *number = strtod(string_without_command, &pointer_to_end_of_number);
    }

    else {
        *number = strtod(string_without_command, &pointer_to_end_of_number) * DEGREE_ACCURACY;
    }

    if (pointer_to_end_of_number == string_without_command + arg_len) {
        return true;
    }

    return false;
}

int strncmp_case_insensitive(const char *str_1, const char *str_2, size_t number_comparisons)
{
    MYASSERT(str_1 != NULL, NULL_POINTER_PASSED_TO_FUNC, return -2);
    MYASSERT(str_2 != NULL, NULL_POINTER_PASSED_TO_FUNC, return -2);

    for(size_t index_comparisons = 0;index_comparisons < number_comparisons; index_comparisons++)
    {
        int compairing_symbol_str_1 = tolower(str_1[index_comparisons]);
        int compairing_symbol_str_2 = tolower(str_2[index_comparisons]);

        if(compairing_symbol_str_1 == compairing_symbol_str_2) {
            continue;
        }

        if(compairing_symbol_str_1 < compairing_symbol_str_2) {
            return -1;
        }

        return 1;
    }

    return 0;
}
