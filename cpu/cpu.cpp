#include "cpu.h"
#include "../libraries/Stack/myassert.h"
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

    while(true)
    {
        int code_operator = pop_from_bytecode_buffer(&bytecode_info);

        int push_value  = 0;
        int pop_value_1 = 0;
        int pop_value_2 = 0;

        switch (code_operator & ~COMMAND_ARGS_REGISTER_AND_NUMBER)
        {
            case HLT:
                stack_destructor(stk);
                bytecode_parametrs_destructor(&bytecode_info);

                return NO_ERROR;

            case PUSH:
                if (code_operator & COMMAND_ARGS_REGISTER)
                {
                    int reg = pop_from_bytecode_buffer(&bytecode_info);

                    if (1 <= reg && reg <= 4)
                        push_value = regs[reg - 1];

                    else
                    {
                        printf("ERROR: INCORRECT NUMBER REG IN PUSH\n");
                        return INVALID_OPERATOR;
                    }
                }

                else if (code_operator & COMMAND_ARGS_NUMBER)
                    push_value = pop_from_bytecode_buffer(&bytecode_info);

                else
                {
                    printf("ERROR: INCORRECT NUMBER ARGS OF PUSH\n");
                    return INVALID_OPERATOR;
                }

                push(stk, push_value * DEGREE_ACCURACY);
                break;

            case SUB:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 - pop_value_2);
                break;

            case DIV:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, DEGREE_ACCURACY * pop_value_1 / pop_value_2);
                break;

            case IN:
                scanf("%d", &push_value);
                push(stk, push_value * DEGREE_ACCURACY);
                break;

            case OUT:
                pop(stk, &pop_value_1);
                printf("%g\n", (float) pop_value_1 / ((float) DEGREE_ACCURACY));
                break;

            case MUL:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 * pop_value_2 / DEGREE_ACCURACY);
                break;

            case ADD:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 + pop_value_2);
                break;

            case SQRT:
                pop(stk, &pop_value_1);
                push(stk, (int) sqrt(pop_value_1 * DEGREE_ACCURACY));
                break;

            case SIN:
                pop(stk, &pop_value_1);
                push(stk, (int) (sin((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
                break;

            case COS:
                pop(stk, &pop_value_1);
                push(stk, (int) (cos((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
                break;

            case POP:
                pop(stk, &pop_value_1);

                if (code_operator & COMMAND_ARGS_REGISTER)
                {
                    int reg = pop_from_bytecode_buffer(&bytecode_info);

                    if (1 <= reg && reg <= 4)
                        regs[reg - 1] = (int) ((float) pop_value_1 / ((float) DEGREE_ACCURACY));

                    else
                    {
                        printf("ERROR: INCORRECT NUMBER REG IN POP\n");
                        return INVALID_OPERATOR;
                    }
                }

                else
                {
                    printf("ERROR: INCORRECT NUMBER ARGS OF POP\n");
                    return INVALID_OPERATOR;
                }

                break;

            default:
                printf("ERROR! Incorrect command: \"%d\"\n", code_operator);
                return INVALID_OPERATOR;
        }
        code_operator = INVALID_OPERATOR;
    }

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
