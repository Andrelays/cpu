#include "assembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Stack/stack.h"
#include <string.h>
#include <ctype.h>

ssize_t assembler (assem_parametrs *assem, FILE *source_code_file_pointer, FILE *byte_code_file_pointer, FILE *listening_file_pointer)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_code_file_pointer   != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(byte_code_file_pointer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    rewind(byte_code_file_pointer);
    assem->buffer_position = 0;

    struct text_parametrs source_code = {
        .string_array = NULL,
        .buffer       = NULL,
        .number_lines = 0
    };

    constructor(&source_code, source_code_file_pointer);

    replace_newline_char_and_comments(source_code.buffer);

    for (size_t position = 0; position < source_code.number_lines; position++)
    {
        const char *string = ((source_code.string_array)[position]).string_pointer;

        if (check_is_empty_string(string) || check_is_label(string, assem))
            continue;

        if (put_command_id_in_buffer(string, assem, listening_file_pointer))
            return INVALID_OPERATOR;
    }

    fwrite(assem->bytecode_buffer, sizeof(int), (size_t) assem->buffer_position, byte_code_file_pointer);

    fprintf(listening_file_pointer, "--------------------------\n");

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
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    if (*string != '\n' && *string != '\0' && *string != '\r')
        return false;

    return true;
}

bool check_is_label(const char *string, assem_parametrs *assem)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    ssize_t label_index = 0;
    char symbol_after_label = 0;

    if (sscanf(string, "%ld%c", &label_index, &symbol_after_label) && label_index >= 0 && label_index <= 9, symbol_after_label == ':')
    {
        assem->labels[label_index] = (int) assem->buffer_position;

        return true;
    }

    return false;
}

ssize_t put_command_id_in_buffer(const char *string, assem_parametrs *assem, FILE *listening_file_pointer)
{
    MYASSERT(string                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    short command_id = INVALID_OPERATOR;
    int number       = 0;
    char reg         = 0;

    #define DEF_COMMAND(command, id, number_args, ...)                                                      \
    do {                                                                                                    \
        if (strncmp(string, #command, sizeof #command - 1) == 0)                                            \
        {                                                                                                   \
            command_id = command;                                                                           \
            const char *string_without_command = string + sizeof #command;                                  \
                                                                                                            \
            if(!check_command_args(string_without_command, number_args, &command_id, &number, &reg, assem)) \
                return INVALID_OPERATOR;                                                                    \
                                                                                                            \
            push_in_bytecode_buffer(assem, command_id);                                                     \
                                                                                                            \
            if (number_args > 0 && command_id & COMMAND_ARGS_REGISTER)                                      \
                push_in_bytecode_buffer(assem, reg - 'a' + 1);                                              \
                                                                                                            \
            if (number_args > 0 && command_id & COMMAND_ARGS_NUMBER)                                        \
                push_in_bytecode_buffer(assem, number);                                                     \
                                                                                                            \
            output_to_listening_file(string, number_args, listening_file_pointer, assem);                   \
                                                                                                            \
            return NO_ERROR;                                                                                \
        }                                                                                                   \
    } while(0);

    #include "../commands.h"

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

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg, assem_parametrs *assem)
{
    char symbol_after_label = 0;

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

        else if(sscanf(string_without_command, "%d%c", number, &symbol_after_label), symbol_after_label == ':')
        {
            *number = assem->labels[*number];
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

    memset(assem->labels, -1, (size_t) assem->labels_size * sizeof(int));

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

void output_to_listening_file(const char *string, size_t number_args, FILE *listening_file_pointer, assem_parametrs *assem)
{
    MYASSERT(assem                  != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(assem->bytecode_buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(listening_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(string                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    fprintf(listening_file_pointer, "%-10s( ", string);

    for (ssize_t index_position = assem->buffer_position - (ssize_t) number_args - 1; index_position < assem->buffer_position; index_position++)
        fprintf(listening_file_pointer, "%2d ", assem->bytecode_buffer[index_position]);

    fprintf(listening_file_pointer, ") %p\n", assem->bytecode_buffer + assem->buffer_position - number_args - 1);
}

