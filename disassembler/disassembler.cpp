#include "disassembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <math.h>
#include <string.h>

errors_code disassembler(FILE *byte_code_file_pointer, FILE *source_code_file_pointer)
{
    MYASSERT(byte_code_file_pointer   != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    struct bytecode_parametrs bytecode_info = {
        .buffer              = NULL,
        .buffer_size         = 0,
        .buffer_position     = 0
    };

    bytecode_parametrs_constructor(byte_code_file_pointer, &bytecode_info);

    #define DEF_COMMAND(command, id, number_args, ...)                                      \
        case (id & ~COMMAND_ARGS_ALL):                                                      \
        {                                                                                   \
            fprintf(source_code_file_pointer, "%s ", #command);                             \
                                                                                            \
            if (number_args == 1)                                                           \
            {                                                                               \
                int arg = pop_from_bytecode_buffer(&bytecode_info);                         \
                                                                                            \
                define_and_print_arg(source_code_file_pointer, code_operator, arg);         \
            }                                                                               \
                                                                                            \
            fprintf(source_code_file_pointer, "\n");                                        \
                                                                                            \
            break;                                                                          \
        }

    while(bytecode_info.buffer_position < bytecode_info.buffer_size)
    {
        int code_operator = pop_from_bytecode_buffer(&bytecode_info);

        switch (code_operator & ~COMMAND_ARGS_ALL)
        {
            #include "../commands.h"

            default:
                printf("ERROR! Incorrect command: \"%d\" buffer_position = %lu\n", code_operator, bytecode_info.buffer_position);
                return INVALID_OPERATOR;
        }

        code_operator = INVALID_OPERATOR;
    }

    #undef DEF_COMMAND

    bytecode_parametrs_destructor(&bytecode_info);

    return ASSERT_NO_ERROR;
}

errors_code bytecode_parametrs_constructor(FILE *byte_code_pointer, bytecode_parametrs *bytecode_info)
{
    MYASSERT(byte_code_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(bytecode_info     != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    size_t file_size = determine_size(byte_code_pointer);

    bytecode_info->buffer = (int *)calloc(file_size + 1, sizeof(char));
    MYASSERT(bytecode_info->buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    bytecode_info->buffer_size = fread(bytecode_info->buffer, sizeof(char), file_size, byte_code_pointer) / sizeof(int);

    bytecode_info->buffer_position = 0;

    return ASSERT_NO_ERROR;
}

errors_code bytecode_parametrs_destructor(bytecode_parametrs *bytecode_info)
{
    MYASSERT(bytecode_info         != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(bytecode_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    free(bytecode_info->buffer);
    bytecode_info->buffer = NULL;

    bytecode_info->buffer_position = 0;
    bytecode_info->buffer_size = 0;

    return ASSERT_NO_ERROR;
}

int pop_from_bytecode_buffer(bytecode_parametrs *bytecode_info)
{
    MYASSERT(bytecode_info         != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(bytecode_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    if (bytecode_info->buffer_position >= bytecode_info->buffer_size)
    {
        printf("ERROR! Pop from an emptystack\n");
        return 0;
    }

    int pop_value = bytecode_info->buffer[bytecode_info->buffer_position];

    ++bytecode_info->buffer_position;

    return pop_value;
}

errors_code define_and_print_arg(FILE *source_code_file_pointer, int code_operator, int arg)
{
    MYASSERT(source_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    switch(code_operator & COMMAND_ARGS_ALL)
    {
        case(COMMAND_ARGS_MEMORY_REGISTER):
        {
            int reg = arg;

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS) {
                fprintf(source_code_file_pointer, "[r%cx]", reg + 'a' - 1);
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_NUMBER):
        {
            int number_memory_cell = arg;

            if(number_memory_cell > 0) {
                fprintf(source_code_file_pointer, "[%d]", number_memory_cell);
            }

            else {
                printf("ERROR: INCORRECT NUMBER MEMORY CELL\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_REGISTER):
        {
            int reg = arg;

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS) {
                fprintf(source_code_file_pointer, "r%cx", reg + 'a' - 1);
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_NUMBER):
        {
            fprintf(source_code_file_pointer, "%d", arg);

            break;
        }

        default:
        {
            printf("ERROR: INCORRECT NUMBER ARGS\n");
            return INVALID_OPERATOR;
        }
    }

    return ASSERT_NO_ERROR;
}
