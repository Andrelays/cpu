#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "cpu.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"

errors_code processor(FILE *byte_code_file_pointer, FILE *logs_file_pointer)
{
    MYASSERT(byte_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(logs_file_pointer      != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    Global_logs_pointer = logs_file_pointer;

    struct processor_parametrs processor_info = {};

    processor_parametrs_constructor(byte_code_file_pointer, &processor_info);

    int   *memory = processor_info.memory;
    stack *stk    = processor_info.stk;

    while(processor_info.buffer_position < processor_info.buffer_size)
    {
        int code_operator = pop_from_bytecode_buffer(&processor_info);

        int  *pointer_to_arg     = NULL;
        int   push_value         = 0;
        int   pop_value_1        = 0;
        int   pop_value_2        = 0;

        check_args(code_operator, &processor_info, &pointer_to_arg);

        #define DEF_COMMAND(command, id, number_args, ...)                                      \
        case (id & ~COMMAND_ARGS_ALL):                                                          \
        {                                                                                       \
            __VA_ARGS__                                                                         \
            break;                                                                              \
        }


        // printf("%d\n", code_operator);

        switch (code_operator & ~COMMAND_ARGS_ALL)
        {
            #include "../commands.h"

            default:
                printf(RED "ERROR! Incorrect command: \"%d\" buffer_position = %lu\n" RESET_COLOR, code_operator, processor_info.buffer_position);
                return INVALID_OPERATOR;
        }

        code_operator = INVALID_OPERATOR;

        #undef DEF_COMMAND
    }

    printf(RED "ERROR: Not HLT\n" RESET_COLOR);

    processor_parametrs_destructor(&processor_info);

    return INVALID_OPERATOR;
}

errors_code processor_parametrs_constructor(FILE *byte_code_file_pointer, processor_parametrs *processor_info)
{
    MYASSERT(byte_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(processor_info         != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    size_t file_size = determine_size(byte_code_file_pointer);

    processor_info->buffer = (int *)calloc(file_size + 1,       sizeof(char));
    processor_info->regs   = (int *)calloc(NUMBER_OF_REGISTERS, sizeof(int));
    processor_info->memory = (int *)calloc(MEMORY_SIZE,         sizeof(int));

    MYASSERT(processor_info->buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);
    MYASSERT(processor_info->regs   != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);
    MYASSERT(processor_info->memory != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    processor_info->buffer_size = fread(processor_info->buffer, sizeof(char), file_size, byte_code_file_pointer) / sizeof(int);
    processor_info->buffer_position = 0;

    processor_info->stk = get_pointer_stack();
    STACK_CONSTRUCTOR(processor_info->stk);

    return ASSERT_NO_ERROR;
}

errors_code processor_parametrs_destructor(processor_parametrs *processor_info)
{
    MYASSERT(processor_info         != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(processor_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    free(processor_info->buffer);
    free(processor_info->regs);
    free(processor_info->memory);

    processor_info->buffer = NULL;
    processor_info->regs   = NULL;
    processor_info->memory = NULL;

    processor_info->buffer_position = 0;
    processor_info->buffer_size     = 0;

    stack_destructor(processor_info->stk);

    SDL_DestroyRenderer(processor_info->renderer);
    SDL_DestroyWindow  (processor_info->window);
    SDL_Quit();

    return ASSERT_NO_ERROR;
}

int pop_from_bytecode_buffer(processor_parametrs *processor_info)
{
    MYASSERT(processor_info         != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(processor_info->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    if (processor_info->buffer_position >= processor_info->buffer_size)
    {
        printf(RED "ERROR! Pop from an empty bytecode_buffer\n" RESET_COLOR);
        return 0;
    }

    int pop_value = processor_info->buffer[processor_info->buffer_position];

    ++processor_info->buffer_position;

    return pop_value;
}

bool check_args(int code_operator, processor_parametrs *processor_info, int **pointer_to_arg)
{
    MYASSERT(processor_info     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(pointer_to_arg     != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    int number_memory_cell = 0;
    int number_reg         = 0;
    int *regs              = processor_info->regs;
    int *memory            = processor_info->memory;

    if (!(code_operator & COMMAND_ARGS_ALL)) {
        return true;
    }

    if (code_operator & COMMAND_ARGS_IMMEDIATE)
    {
        *pointer_to_arg = processor_info->buffer + processor_info->buffer_position;
        ++processor_info->buffer_position;
    }

    if (code_operator & COMMAND_ARGS_REGISTER)
    {
        number_reg = pop_from_bytecode_buffer(processor_info) - 1;

        if (0 <= number_reg && number_reg < NUMBER_OF_REGISTERS) {
            *pointer_to_arg = regs + number_reg;
        }

        else {
            printf(RED "ERROR: INCORRECT NUMBER REG\n" RESET_COLOR);
            return false;
        }
    }

    if (code_operator & COMMAND_ARGS_MEMORY)
    {
        if (code_operator & COMMAND_ARGS_REGISTER) {
            number_memory_cell = **pointer_to_arg / DEGREE_ACCURACY;
        }

        else {
            number_memory_cell = **pointer_to_arg;
        }

        if(0 <= number_memory_cell && number_memory_cell < MEMORY_SIZE) {
            *pointer_to_arg = memory + number_memory_cell;
        }

        else {
            printf(RED "ERROR: INCORRECT NUMBER MEMORY CELL: %d\n" RESET_COLOR, number_memory_cell);
            return false;
        }
    }

    return true;
}
