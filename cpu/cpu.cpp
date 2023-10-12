#include "cpu.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <math.h>

FILE *Global_input_pointer = NULL;
int Global_degree_accuracy = 100;

ssize_t processor(stack *stk)
{
    MYASSERT(Global_input_pointer  != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(stk                   != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    int code_operator = NO_OPERATOR;

    int push_value  = 0;
    int pop_value_1 = 0;
    int pop_value_2 = 0;

    while(true)
    {
        fscanf(Global_input_pointer, "%d", &code_operator);

        switch (code_operator)
        {
            case HLT:
                return NO_ERROR;

            case PUSH:
                fscanf(Global_input_pointer, "%d", &push_value);
                push(stk, push_value * Global_degree_accuracy);
                break;

            case SUB:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 - pop_value_2);
                break;

            case DIV:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, Global_degree_accuracy * pop_value_1 / pop_value_2);
                break;

            case IN:
                scanf("%d", &push_value);
                push(stk, push_value * Global_degree_accuracy);
                break;

            case OUT:
                pop(stk, &pop_value_1);
                printf("%g\n", (float) pop_value_1 / ((float) Global_degree_accuracy));
                break;

            case MUL:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 * pop_value_2 / Global_degree_accuracy);
                break;

            case ADD:
                pop(stk, &pop_value_2);
                pop(stk, &pop_value_1);
                push(stk, pop_value_1 + pop_value_2);
                break;

            case SQRT:
                pop(stk, &pop_value_1);
                push(stk, (int) sqrt(pop_value_1 * Global_degree_accuracy));
                break;

            case SIN:
                pop(stk, &pop_value_1);
                push(stk, (int) (sin(pop_value_1 / Global_degree_accuracy) * Global_degree_accuracy));
                break;

            case COS:
                pop(stk, &pop_value_1);
                push(stk, (int) (cos(pop_value_1 / Global_degree_accuracy) * Global_degree_accuracy));
                break;

            default:
                printf("ERROR! Incorrect command: \"%d\"\n", code_operator);
                return INVALID_COMMAND;
        }

        code_operator = NO_OPERATOR;
    }

    return NO_ERROR;
}

