#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "cpu.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <SDL2/SDL.h>

errors_code processor(FILE *byte_code_file_pointer, FILE *logs_file_pointer)
{
    MYASSERT(byte_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(logs_file_pointer      != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    SDL_Renderer *renderer = NULL;
    SDL_Window *window     = NULL;

    Global_logs_pointer = logs_file_pointer;

    int *memory = (int *) calloc(MEMORY_SIZE, sizeof(int));
    int regs[NUMBER_OF_REGISTERS] = {};

    stack *stk = get_pointer_stack();
    STACK_CONSTRUCTOR(stk);

    struct bytecode_parametrs bytecode_info = {
        .buffer              = NULL,
        .buffer_size         = 0,
        .buffer_position     = 0
    };

    bytecode_parametrs_constructor(byte_code_file_pointer, &bytecode_info);

    #define DEF_COMMAND(command, id, number_args, ...)  \
        case (id & ~COMMAND_ARGS_ALL):                  \
            __VA_ARGS__                                 \
            break;

    while(bytecode_info.buffer_position < bytecode_info.buffer_size)
    {
        int code_operator = pop_from_bytecode_buffer(&bytecode_info);

        int number_memory_cell = 0;
        int push_value         = 0;
        int pop_value_1        = 0;
        int pop_value_2        = 0;

        switch (code_operator & ~COMMAND_ARGS_ALL)
        {
            #include "../commands.h"

            default:
                printf(RED "ERROR! Incorrect command: \"%d\" buffer_position = %lu\n" RESET_COLOR, code_operator, bytecode_info.buffer_position);
                return INVALID_OPERATOR;
        }

        code_operator = INVALID_OPERATOR;
    }

    #undef DEF_COMMAND

    printf(RED "ERROR: Not HLT\n" RESET_COLOR);

    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    free(memory);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return INVALID_OPERATOR;
}

errors_code bytecode_parametrs_constructor(FILE *byte_code_file_pointer, bytecode_parametrs *bytecode_info)
{
    MYASSERT(byte_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(bytecode_info          != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    size_t file_size = determine_size(byte_code_file_pointer);

    bytecode_info->buffer = (int *)calloc(file_size + 1, sizeof(char));
    MYASSERT(bytecode_info->buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    bytecode_info->buffer_size = fread(bytecode_info->buffer, sizeof(char), file_size, byte_code_file_pointer) / sizeof(int);

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
        printf(RED "ERROR! Pop from an emptystack\n" RESET_COLOR);
        return 0;
    }

    int pop_value = bytecode_info->buffer[bytecode_info->buffer_position];

    ++bytecode_info->buffer_position;

    return pop_value;
}
