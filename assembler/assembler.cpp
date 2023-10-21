#include "assembler.h"
#include "../enum.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

errors_code assembler(assem_parametrs *assem, FILE *source_code_file_pointer, FILE *byte_code_file_pointer, FILE *listening_file_pointer)
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

    return ASSERT_NO_ERROR;
}

errors_code put_command_id_in_buffer(const char *string, assem_parametrs *assem, FILE *listening_file_pointer)
{
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    short command_id = NO_OPERATOR;
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
            {                                                                                               \
                printf("ERROR COMMAND: %s\n", string);                                                      \
                return INVALID_OPERATOR;                                                                    \
            }                                                                                               \
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
            return ASSERT_NO_ERROR;                                                                         \
        }                                                                                                   \
    } while(0);

    #include "../commands.h"

    #undef DEF_COMMAND

    printf("ERROR COMMAND: %s\n", string);

    return INVALID_OPERATOR;
}

bool check_command_args(const char *string_without_command, size_t number_args, short *command_id, int *number, char *reg, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(string_without_command     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(command_id                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(number                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(reg                        != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    // char symblo_after_label = 0;
    char label[MAX_SIZE_LABELS] = {};

    if (number_args == 0)
        return true;

    if (number_args == 1)
    {
        if (sscanf(string_without_command, "r%cx", reg) && *reg >= 'a' && *reg <= 'd')
        {
            *command_id |= COMMAND_ARGS_REGISTER;

            return true;
        }

        else if(sscanf(string_without_command, ":%s", label))
        {
            *command_id |= COMMAND_ARGS_NUMBER;
            *number = (int) find_label(label, assem);

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

errors_code push_in_bytecode_buffer(assem_parametrs *assem, int number)
{
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    check_size_buffer(assem);

    assem->bytecode_buffer[assem->buffer_position] = number;
    ++assem->buffer_position;

    return ASSERT_NO_ERROR;
}

errors_code check_size_buffer(assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    if (assem->buffer_position < assem->buffer_size)
        return ASSERT_NO_ERROR;

    const int COEFFICIENT_BUFFER_INCREASE = 2;

    assem->buffer_size *= COEFFICIENT_BUFFER_INCREASE;

    assem->bytecode_buffer = (int *) realloc (assem->bytecode_buffer, (size_t) assem->buffer_size * sizeof(int));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    for (ssize_t index = assem->buffer_position; index < assem->buffer_size;index++)
        assem->bytecode_buffer[index] = 0;

    return ASSERT_NO_ERROR;
}

ssize_t find_label(char *label, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(label                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);

    for (ssize_t label_index = 0; label_index < assem->labels_index_position; label_index++)
        if (strcmp((assem->labels_array[label_index]).label_name, label) == 0)
            return (assem->labels_array[label_index]).label_position;

    return -1;
}

bool check_is_label(const char *string, assem_parametrs *assem)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    char label[MAX_SIZE_LABELS] = {};
    // char symbol_after_label = 0;

    if (sscanf(string, ":%s", label))
    {
        //check_colon_in_label();

        if (assem->labels_index_position >= SIZE_ARRAY_LABELS)
        {
            printf("ERROR: Too many labels, max: %ld\n", SIZE_ARRAY_LABELS);
            return false;
        }

        (assem->labels_array[assem->labels_index_position]).label_position = assem->buffer_position;

        strncpy((assem->labels_array[assem->labels_index_position]).label_name, label, MAX_SIZE_LABELS);

        ++assem->labels_index_position;

        return true;
    }

    return false;
}

errors_code assem_parametrs_constructor(assem_parametrs *assem)
{
    MYASSERT(assem != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    assem->bytecode_buffer = (int *) calloc(INITIAL_BUFFER_SIZE, sizeof(int));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    assem->buffer_position = 0;
    assem->buffer_size = INITIAL_BUFFER_SIZE;

    assem->labels_index_position = 0;

    return ASSERT_NO_ERROR;
}

errors_code assem_parametrs_destructor(assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    assem->buffer_position = -1;
    assem->buffer_size     = -1;

    free(assem->bytecode_buffer);

    assem->bytecode_buffer = NULL;
    assem = NULL;

    return ASSERT_NO_ERROR;
}

errors_code output_to_listening_file(const char *string, size_t number_args, FILE *listening_file_pointer, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(listening_file_pointer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    fprintf(listening_file_pointer, "%-10s(%d", string, *(assem->bytecode_buffer + assem->buffer_position - number_args - 1));

    for (ssize_t index_position = assem->buffer_position - (ssize_t) number_args; index_position < assem->buffer_position; index_position++)
        fprintf(listening_file_pointer, " %d", assem->bytecode_buffer[index_position]);

    fprintf(listening_file_pointer, ") %p\n", assem->bytecode_buffer + assem->buffer_position - number_args - 1);

    return ASSERT_NO_ERROR;
}

errors_code replace_newline_char_and_comments(char *text)
{
    MYASSERT(text != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    while (*text != '\0')
    {
        if (*text == '\r' || *text == '\n' || *text == ';')
            *text = '\0';

        ++text;
    }

    return ASSERT_NO_ERROR;
}

bool check_is_empty_string(const char *string)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    if (*string != '\n' && *string != '\0' && *string != '\r')
        return false;

    return true;
}
