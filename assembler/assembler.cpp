#include "assembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <string.h>
#include <ctype.h>

FILE *Global_input_pointer  = NULL;
FILE *Global_output_pointer = NULL;

ssize_t assembler (struct text_parametrs *source_code)
{
    MYASSERT(Global_input_pointer  != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(Global_output_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    replace_newline_char(source_code->buffer);

    for (size_t line_counter = 0; line_counter < source_code->number_lines; line_counter++)
    {
        const char *string = ((source_code->string_array)[line_counter]).string_pointer;

        if (check_is_empty_string(string))
            continue;

        operators code_operator = find_operator(string);

        if(!check_is_correct_operator(code_operator, line_counter + 1, string))
            return INVALID_COMMAND;

        if(code_operator == PUSH && !check_is_correct_number_push(string, line_counter + 1))
            return INVALID_COMMAND;

        if(code_operator != PUSH)
            fprintf(Global_output_pointer, "%d\n", code_operator);

        else
            fprintf(Global_output_pointer, "%d%s\n", code_operator, string + sizeof "PUSH" - 1);
    }

    return NO_ERROR;
}

void replace_newline_char(char *text)
{
    while (*text != '\0')
    {
        if (*text == '\r' || *text == '\n')
            *text = '\0';

        ++text;
    }
}

operators find_operator (const char *string)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return NO_OPERATOR);

    #define CHECK_OPERATOR_(command)                                                        \
    do {                                                                                    \
        if (strncmp(string, #command, sizeof #command - 1) == 0) return command;            \
    } while(0)

    CHECK_OPERATOR_(HLT);
    CHECK_OPERATOR_(PUSH);
    CHECK_OPERATOR_(SUB);
    CHECK_OPERATOR_(DIV);
    CHECK_OPERATOR_(IN);
    CHECK_OPERATOR_(OUT);
    CHECK_OPERATOR_(MUL);
    CHECK_OPERATOR_(ADD);
    CHECK_OPERATOR_(SQRT);
    CHECK_OPERATOR_(SIN);
    CHECK_OPERATOR_(COS);
    CHECK_OPERATOR_(POP);

    #undef CHECK_OPERATOR_

    return NO_OPERATOR;
}

bool check_is_empty_string(const char *string)
{
    if (*string != '\n' && *string != '\0' && *string != '\r')
        return false;

    return true;
}

bool check_is_correct_operator(operators code_operator, size_t line_number, const char *string)
{
    if (code_operator != NO_OPERATOR)
        return true;

    printf("ERROR! Invalid command name, line %lu: \"%s\"\n", line_number, string);

    return false;
}

bool check_is_correct_number_push(const char *string, size_t line_number)
{
    const char *string_whithout_command = string + sizeof "PUSH";

    while (*string_whithout_command != '\0')
    {
        if (!isspace(*string_whithout_command) && !isdigit(*string_whithout_command))
        {
            printf("ERROR! Invalid PUSH number, line %lu: \"%s\"\n", line_number, string);

            return false;
        }

        ++string_whithout_command;
    }

    return true;
}
