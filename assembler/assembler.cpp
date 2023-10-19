#include "assembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <string.h>
#include <ctype.h>

ssize_t assembler (assem_parametrs *assem, FILE *source_code_pointer, FILE *byte_code_pointer)
{
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_code_pointer    != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(byte_code_pointer      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    struct text_parametrs source_code = {
        .string_array = NULL,
        .buffer       = NULL,
        .number_lines = 0
    };

    constructor(&source_code, source_code_pointer);

    replace_newline_char_and_comments(source_code.buffer);

    for (size_t position = 0; position < source_code.number_lines; position++)
    {
        const char *string = ((source_code.string_array)[position]).string_pointer;

        if (check_is_empty_string(string))
            continue;

        if (put_command_id_in_buffer(string, assem))
            return INVALID_OPERATOR;
    }

    fwrite(assem->bytecode_buffer, sizeof(int), (size_t) assem->buffer_position, byte_code_pointer);

    // for (ssize_t i = 0; i < assem->buffer_position; i ++)
    //     fprintf(byte_code_pointer, "\n%d\n", assem->bytecode_buffer[i]);

    destructor(&source_code);

    return NO_ERROR;
}

void replace_newline_char_and_comments(char *text)
{
    while (*text != '\0')
    {
        if (*text == '\r' || *text == '\n' || *text == ';')
            *text = '\0';

        ++text;
    }
}

bool check_is_empty_string(const char *string)
{
    if (*string != '\n' && *string != '\0' && *string != '\r')
        return false;

    return true;
}

ssize_t put_command_id_in_buffer(const char *string, assem_parametrs *assem)
{
    MYASSERT(string                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    short command_id = INVALID_OPERATOR;
    int number       = 0;
    char reg         = 0;

    #define DEF_COMMAND(command, id, number_args, ...)                                                  \
    do {                                                                                                \
        if (strncmp(string, #command, sizeof #command - 1) == 0)                                        \
        {                                                                                               \
            command_id = command;                                                                       \
            const char *string_without_command = string + sizeof #command;                              \
                                                                                                        \
            if(!check_command_args(string_without_command, number_args, &command_id, &number, &reg))    \
                return INVALID_OPERATOR;                                                                \
                                                                                                        \
            push_in_bytecode_buffer(assem, command_id);                                                 \
                                                                                                        \
            if (number_args > 0 && command_id & COMMAND_ARGS_REGISTER)                                  \
                push_in_bytecode_buffer(assem, reg - 'a' + 1);                                          \
                                                                                                        \
            if (number_args > 0 && command_id & COMMAND_ARGS_NUMBER)                                    \
                push_in_bytecode_buffer(assem, number);                                                 \
                                                                                                        \
            return NO_ERROR;                                                                            \
        }                                                                                               \
    } while(0)

    DEF_COMMAND(HLT,   1, 0);
    DEF_COMMAND(PUSH,  2, 1);
    DEF_COMMAND(SUB,   3, 0);
    DEF_COMMAND(DIV,   4, 0);
    DEF_COMMAND(IN,    5, 0);
    DEF_COMMAND(OUT,   6, 0);
    DEF_COMMAND(MUL,   7, 0);
    DEF_COMMAND(ADD,   8, 0);
    DEF_COMMAND(SQRT,  9, 0);
    DEF_COMMAND(SIN,  10, 0);
    DEF_COMMAND(COS,  11, 0);
    DEF_COMMAND(POP,  12, 1);

    #undef DEF_COMMAND

    printf("ERROR COMMAND: %s\n", string);

    return INVALID_OPERATOR;
}

void push_in_bytecode_buffer(assem_parametrs *assem, int number)
{
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    // printf("%d ", number);

    check_size_buffer(assem);

    assem->bytecode_buffer[assem->buffer_position] = number;
    ++assem->buffer_position;
}

void check_size_buffer(assem_parametrs *assem)
{
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    if (assem->buffer_position < assem->buffer_size)
        return;

    const int COEFFICIENT_BUFFER_INCREASE = 2;

    assem->buffer_size *= COEFFICIENT_BUFFER_INCREASE;

    assem->bytecode_buffer = (int *) realloc (assem->bytecode_buffer, (size_t) assem->buffer_size * sizeof(int));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return);

    for (ssize_t index = assem->buffer_position; index < assem->buffer_size;index++)
        assem->bytecode_buffer[index] = 0;
}

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg)
{
    if (number_args == 0)
        return true;

    if (number_args == 1)
    {
        if (sscanf(string_without_command, "r%cx", reg) && *reg >= 'a' && *reg <= 'd')
        {
            *command_id |= COMMAND_ARGS_REGISTER;
            return true;
        }

        else if(sscanf(string_without_command, "%d", number))
        {
            *command_id |= COMMAND_ARGS_NUMBER;
            return true;
        }

        printf("ERROR COMMAND ARGS: %s\n", string_without_command);
    }

    return false;
}

ssize_t assem_parametrs_constructor(assem_parametrs *assem)
{
    MYASSERT(assem != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    const ssize_t INITIAL_BUFFER_SIZE = 100;

    assem->bytecode_buffer = (int *) calloc(INITIAL_BUFFER_SIZE, sizeof(int));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    assem->buffer_position = 0;
    assem->buffer_size = INITIAL_BUFFER_SIZE;

    return NO_ERROR;
}

ssize_t assem_parametrs_destructor(assem_parametrs *assem)
{
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    assem->buffer_position = -1;
    assem->buffer_size     = -1;

    free(assem->bytecode_buffer);

    assem->bytecode_buffer = NULL;
    assem = NULL;

    return NO_ERROR;
}

