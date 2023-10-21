#include "cpu.h"
#include "../libraries/Onegin/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <math.h>
#include <stdio.h>

const int DEGREE_ACCURACY = 100;

ssize_t processor(FILE *byte_code_pointer, FILE *logs_pointer)
{
    MYASSERT(byte_code_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(logs_pointer      != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    Global_logs_pointer = logs_pointer;

    int regs[4] = {};

    stack *stk = get_pointer_stack();
    STACK_CONSTRUCTOR(stk);

    struct bytecode_parametrs bytecode_info = {
        .buffer              = NULL,
        .buffer_size         = 0,
        .buffer_position     = 0
    };

    bytecode_parametrs_constructor(byte_code_pointer, &bytecode_info);


    // printf("buffer_size %lu\n", bytecode_info.buffer_size);

    //bytecode.bytecode_buffer = (int *) input_data(byte_code_pointer);

    // for (int i = 0; i < 100; i++)
    //     printf("%d\n", pop_from_bytecode_buffer(&bytecode_info));

    // for (int i = 0; i < NUMBER_OPERATIONS - 1; i++)
    //     printf("%s %d %ld %ld\n", OPERATIONS[i].name, OPERATIONS[i].id, OPERATIONS[i].size, OPERATIONS[i].number_args);

    #define DEF_COMMAND(command, id, number_args, ...)  \
        case (id & ~COMMAND_ARGS_ALL):                  \
            __VA_ARGS__                                 \
            break;

    while(true)
    {
        // printf("buffer_position = %ld\n", bytecode_info.buffer_position);

        int code_operator = pop_from_bytecode_buffer(&bytecode_info);

        // printf("buffer_position = %ld\n", bytecode_info.buffer_position);

        int push_value  = 0;
        int pop_value_1 = 0;
        int pop_value_2 = 0;

        switch (code_operator & ~COMMAND_ARGS_ALL)
        {
            #include "../commands.h"

            default:
                printf("ERROR! Incorrect command: \"%d\" buffer_position = %ld\n", code_operator, bytecode_info.buffer_position);
                return INVALID_OPERATOR;
        }
        code_operator = INVALID_OPERATOR;
    }

    #undef DEF_COMMAND

    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    return NO_ERROR;
}

ssize_t bytecode_parametrs_constructor(FILE *byte_code_pointer, bytecode_parametrs *bytecode_info)
{
    MYASSERT(byte_code_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(bytecode_info     != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    size_t file_size = determine_size(byte_code_pointer) / sizeof(int);

    bytecode_info->buffer = (int *)calloc(file_size, sizeof(int));
    MYASSERT(bytecode_info->buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    bytecode_info->buffer_size = (ssize_t) fread(bytecode_info->buffer, sizeof(int), file_size, byte_code_pointer);

    bytecode_info->buffer_position = 0;

    return NO_ERROR;
}

ssize_t bytecode_parametrs_destructor(bytecode_parametrs *bytecode_info)
{
    MYASSERT(bytecode_info         != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(bytecode_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    free(bytecode_info->buffer);
    bytecode_info->buffer = NULL;

    bytecode_info->buffer_position = -1;
    bytecode_info->buffer_size = -1;

    bytecode_info = NULL;

    return NO_ERROR;
}

int pop_from_bytecode_buffer(bytecode_parametrs *bytecode_info)
{
    MYASSERT(bytecode_info         != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(bytecode_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    if (bytecode_info->buffer_position >= bytecode_info->buffer_size)
        return 0;

    int pop_value = bytecode_info->buffer[bytecode_info->buffer_position];

    ++bytecode_info->buffer_position;

    return pop_value;
}
