#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <climits>
#include "assembler.h"
#include "../enum.h" 

errors_code assembler(assem_parametrs *assem, FILE *source_code_file_pointer, FILE *byte_code_file_pointer, FILE *listing_file_pointer)
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

    text_parametrs_constructor(&source_code, source_code_file_pointer);

    replace_newline_char_and_comments(source_code.buffer);

    for (size_t position = 0; position < source_code.number_lines; position++)
    {
        char *string = ((source_code.string_array)[position]).string_pointer;

        if (check_is_empty_string(string) || check_is_label(string, assem))
            continue;

        if (put_command_id_in_buffer(string, assem, listing_file_pointer))
            return INVALID_OPERATOR;
    }

    fwrite(assem->bytecode_buffer, sizeof(int), (size_t) assem->buffer_position, byte_code_file_pointer);

    putn('-', 60, listing_file_pointer);

    text_parametrs_destructor(&source_code);

    return ASSERT_NO_ERROR;
}

errors_code put_command_id_in_buffer(char *string, assem_parametrs *assem, FILE *listing_file_pointer)
{
    MYASSERT(string                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    short command_id = NO_OPERATOR;
    ssize_t arg_len  = 0;
    int number       = 0;
    char reg         = 0;

    #define DEF_COMMAND(command, id, number_args, ...)                                                                      \
    do {                                                                                                                    \
        if (strncmp(string, #command, sizeof #command - 1) == 0)                                                            \
        {                                                                                                                   \
            if(string[sizeof #command - 1] == '\0' || isspace(string[sizeof #command - 1]))                                 \
            {                                                                                                               \
                command_id = id;                                                                                            \
                                                                                                                            \
                char *string_without_command = parsing_string_to_tokens(string, &arg_len, " \r\n\t");                       \
                                                                                                                            \
                if (string_without_command && parsing_string_to_tokens(string_without_command, NULL, " \r\n\t"))            \
                {                                                                                                           \
                    printf("ERROR COMMAND: %s\n", string);                                                                  \
                    return INVALID_OPERATOR;                                                                                \
                }                                                                                                           \
                                                                                                                            \
                if (!check_command_args(string_without_command, number_args, &command_id, &number, &reg, assem, arg_len))   \
                {                                                                                                           \
                    printf("ERROR COMMAND: %s\n", string);                                                                  \
                    return INVALID_OPERATOR;                                                                                \
                }                                                                                                           \
                                                                                                                            \
                push_in_bytecode_buffer(assem, command_id);                                                                 \
                                                                                                                            \
                if (number_args > 0 && command_id & COMMAND_ARGS_REGISTER)                                                  \
                    push_in_bytecode_buffer(assem, reg - 'a' + 1);                                                          \
                                                                                                                            \
                if (number_args > 0 && command_id & COMMAND_ARGS_NUMBER)                                                    \
                    push_in_bytecode_buffer(assem, number);                                                                 \
                                                                                                                            \
                output_to_listing_file(string, number_args, listing_file_pointer, assem);                                   \
                                                                                                                            \
                return ASSERT_NO_ERROR;                                                                                     \
            }                                                                                                               \
        }                                                                                                                   \
    } while(0);

    #include "../commands.h"

    #undef DEF_COMMAND

    printf("ERROR COMMAND: %s\n", string);

    return INVALID_OPERATOR;
}

bool check_command_args(char *string_without_command, size_t number_args, short *command_id, int *number, char *reg, assem_parametrs *assem, ssize_t arg_len)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(command_id                 != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(number                     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(reg                        != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    char label[MAX_SIZE_LABELS] = {};

    if (number_args == 0)
        return true;

    if (!string_without_command)
        return false;

    //printf("%s\n", string_without_command);

    // printf("<%s> len = %ld\n", str_1, token_length);

    if (number_args == 1)
    {
        // ptr = strtok('r')
        // if ptr != NULL
        // check 'x '
        // check 0 <= ptr[1] - 'a' <= MAX_REGISTER

        if (check_is_register(string_without_command, reg, arg_len))
        {
            *command_id |= COMMAND_ARGS_REGISTER;

            return true;
        }

        if(string_without_command[arg_len - 1] == ':')
        {
            strncpy(label, string_without_command, (size_t) arg_len - 1);

            // printf("label = %s\n", label);

            *command_id |= COMMAND_ARGS_NUMBER;
            *number = (int) find_label(label, assem);

            return true;
        }

        if(check_is_number(string_without_command, number, arg_len))
        {
            *command_id |= COMMAND_ARGS_NUMBER;

            return true;
        }

        if(string_without_command[0] == '[' && string_without_command[arg_len - 1] == ']')
        {
            if(check_is_register(string_without_command + 1, reg, arg_len - 2))
            {
                *command_id |= COMMAND_ARGS_MEMORY_REGISTER;

                return true;
            }

            if(check_is_number(string_without_command + 1, number, arg_len - 2))
            {
                *command_id |= COMMAND_ARGS_MEMORY_NUMBER;

                return true;
            }

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

    label_parametrs *labels = assem->labels_array;

    for (ssize_t label_index = 0; label_index < assem->labels_index_position; label_index++)
        if (strcmp((labels[label_index]).label_name, label) == 0)
            return (labels[label_index]).label_position;

    return -1;
}

bool check_is_label(char *string, assem_parametrs *assem)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    ssize_t label_len = 0;
    label_parametrs *labels = assem->labels_array;

    label_len = find_token_size(string, " \r\n\t");

    MYASSERT(label_len > 0, NEGATIVE_VALUE_SIZE_T, return false);

    if (parsing_string_to_tokens(string, NULL, " \r\n\t"))
        return false;



    //printf("<%s> len = %ld\n", string, label_len);

    if (string[label_len - 1] == ':')
    {
        if (assem->labels_index_position >= SIZE_ARRAY_LABELS)
        {
            printf("ERROR: Too many labels, max: %ld\n", SIZE_ARRAY_LABELS);
            return false;
        }

        (labels[assem->labels_index_position]).label_position = assem->buffer_position;

        strncpy((labels[assem->labels_index_position]).label_name, string, (size_t) label_len - 1);

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

errors_code putn(char symbol, size_t number_symbols, FILE *file)
{
    MYASSERT(file != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t counter_symbols = 0; counter_symbols < number_symbols; counter_symbols++)
        putc(symbol, file);

    putc('\n', file);

    return ASSERT_NO_ERROR;
}

char *parsing_string_to_tokens(char *string, ssize_t *token_length, const char *separator)
{
    MYASSERT(string       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);
    MYASSERT(separator    != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);

    char *token = strpbrk(string, separator);

    // printf("string = %s\n", string);

    if (token == NULL)
        return token;

    // printf("strspn = %lu\n", strspn(token, separator));
    token += strspn(token, separator);

    if (*token == '\0')
        return NULL;

    // printf("token = <%s>\n", token);

    if (token_length != NULL)
        *token_length = find_token_size(token, separator);

    return token;
}

ssize_t find_token_size(char *token, const char *separator)
{
    MYASSERT(token        != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(separator    != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);

    ssize_t token_length = 0;

    char *pointer_end_of_token = NULL;

    if ((pointer_end_of_token = strpbrk(token, separator)) != NULL)
        token_length = pointer_end_of_token - token;

    else
        token_length = (ssize_t) strlen(token);

    return token_length;
}

bool check_is_register(char *string_without_command, char *reg, ssize_t arg_len)
{
    if (string_without_command[0] == 'r' && string_without_command[2] == 'x' && arg_len == 3)
    {
        *reg = string_without_command[1];

        if(*reg - 'a' >= 0 && *reg - 'a' <= NUMBER_OF_REGISTERS - 1)
            return true;
    }

    return false;
}

bool check_is_number(char *string_without_command, int *number, ssize_t arg_len)
{
    char *pointer_to_end_of_number = NULL;

    *number = (int) strtol(string_without_command, &pointer_to_end_of_number, 10);

    if (pointer_to_end_of_number == string_without_command + arg_len)
        return true;

    return false;
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
