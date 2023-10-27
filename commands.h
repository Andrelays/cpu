//WARNING HLT 31st command!!!

#define POP_WITH_CHECK(stk, pop_value)              \
 do {                                               \
    if(pop(stk, pop_value) != NO_ERROR)             \
    {                                               \
        printf("ERROR! Pop from an emptystack");    \
        return INVALID_OPERATOR;                    \
    }                                               \
                                                    \
}  while(0)

DEF_COMMAND(HLT,  -1, 0,
{
    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    return ASSERT_NO_ERROR;
})

DEF_COMMAND(PUSH,  2, 1,
{
    switch(code_operator & COMMAND_ARGS_ALL)
    {
        case(COMMAND_ARGS_MEMORY_REGISTER):
        {
            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
            {
                if((number_memory_cell = regs[reg - 1]) < MEMORY_SIZE && number_memory_cell > 0) {
                    push_value = memory[number_memory_cell];
                }

                else {
                    printf("ERROR: INCORRECT NUMBER MEMORY CELL IN PUSH\n");
                    return INVALID_OPERATOR;
                }
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG IN PUSH\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_NUMBER):
        {
            number_memory_cell = pop_from_bytecode_buffer(&bytecode_info);

            if(number_memory_cell < MEMORY_SIZE && number_memory_cell > 0) {
                push_value = memory[number_memory_cell];
            }

            else {
                printf("ERROR: INCORRECT NUMBER MEMORY CELL IN PUSH\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_REGISTER):
        {
            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS) {
                push_value = regs[reg - 1];
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG IN PUSH\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_NUMBER):
        {
            push_value = pop_from_bytecode_buffer(&bytecode_info);

            break;
        }

        default:
        {
            printf("ERROR: INCORRECT NUMBER ARGS OF PUSH\n");
            return INVALID_OPERATOR;
        }
    }

    push(stk, push_value * DEGREE_ACCURACY);
})

DEF_COMMAND(SUB,   3, 0,
{
    POP_WITH_CHECK(stk, &pop_value_2);
    POP_WITH_CHECK(stk, &pop_value_1);

    push(stk, pop_value_1 - pop_value_2);
})

DEF_COMMAND(DIV,   4, 0,
{
    POP_WITH_CHECK(stk, &pop_value_2);

    if (!pop_value_2)
    {
        printf("ERROR! ATTEMPT TO DIV BY 0");
        return INVALID_OPERATOR;
    }

    POP_WITH_CHECK(stk, &pop_value_1);

    push(stk, DEGREE_ACCURACY * pop_value_1 / pop_value_2);
})

DEF_COMMAND(IN,    5, 0,
{
    scanf("%d", &push_value);
    push(stk, push_value * DEGREE_ACCURACY);
})

DEF_COMMAND(OUTC, 6, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    printf("%c", pop_value_1 / DEGREE_ACCURACY);
})

DEF_COMMAND(OUT,   7, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    printf("%g\n", (float) pop_value_1 / ((float) DEGREE_ACCURACY));
})

DEF_COMMAND(MUL,   8, 0,
{
    POP_WITH_CHECK(stk, &pop_value_2);
    POP_WITH_CHECK(stk, &pop_value_1);

    push(stk, pop_value_1 * pop_value_2 / DEGREE_ACCURACY); //TODO check overflow
})

DEF_COMMAND(ADD,   9, 0,
{
    POP_WITH_CHECK(stk, &pop_value_2);
    POP_WITH_CHECK(stk, &pop_value_1);

    push(stk, pop_value_1 + pop_value_2);
})

DEF_COMMAND(SQRT,  10, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    push(stk, (int) sqrt(pop_value_1 * DEGREE_ACCURACY));
})

DEF_COMMAND(SIN,  11, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    push(stk, (int) (sin((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(COS,  12, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    push(stk, (int) (cos((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(POP,  13, 1,
{
    POP_WITH_CHECK(stk, &pop_value_1);

    switch(code_operator & COMMAND_ARGS_ALL)
    {
        case(COMMAND_ARGS_REGISTER):
        {
            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS) {
                regs[reg - 1] = (int) ((float) pop_value_1 / ((float) DEGREE_ACCURACY));
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG IN POP\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_REGISTER):
        {
            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
            {
                if((number_memory_cell = regs[reg - 1]) < MEMORY_SIZE && number_memory_cell > 0) {
                    memory[number_memory_cell] = (int) ((float) pop_value_1 / ((float) DEGREE_ACCURACY));
                }

                else {
                    printf("ERROR: INCORRECT NUMBER MEMORY CELL IN POP\n");
                    return INVALID_OPERATOR;
                }
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG IN POP\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_NUMBER):
        {
            number_memory_cell = pop_from_bytecode_buffer(&bytecode_info);

            if(number_memory_cell < MEMORY_SIZE && number_memory_cell > 0) {
                memory[number_memory_cell] = (int) ((float) pop_value_1 / ((float) DEGREE_ACCURACY));
            }

            else {
                printf("ERROR: INCORRECT NUMBER REG IN POP\n");
                return INVALID_OPERATOR;
            }

            break;
        }

        default:
        {
            printf("ERROR: INCORRECT NUMBER ARGS OF POP\n");
            return INVALID_OPERATOR;
        }
    }
})

DEF_COMMAND(JMP,  14, 1,
{
    bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];
})

#define JUMP_WITH_CONDITION(name, id, sign)                                                         \
DEF_COMMAND(name, id, 1,                                                                            \
{                                                                                                   \
    POP_WITH_CHECK(stk, &pop_value_2);                                                              \
    POP_WITH_CHECK(stk, &pop_value_1);                                                              \
                                                                                                    \
    if (pop_value_1 sign pop_value_2) {                                                             \
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];        \
    }                                                                                               \
                                                                                                    \
    else {                                                                                          \
        ++bytecode_info.buffer_position;                                                            \
    }                                                                                               \
})

JUMP_WITH_CONDITION(JA,  15, >)
JUMP_WITH_CONDITION(JAE, 16, >=)
JUMP_WITH_CONDITION(JB,  17, <)
JUMP_WITH_CONDITION(JBE, 18, <=)
JUMP_WITH_CONDITION(JE,  19, ==)
JUMP_WITH_CONDITION(JNE, 20, !=)

#undef JUMP_WITH_CONDITION

DEF_COMMAND(CALL,  21, 1,
{
    push(stk, (int) bytecode_info.buffer_position + 1);
    bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];
})

DEF_COMMAND(RETURN, 22, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    bytecode_info.buffer_position = pop_value_1;
})

DEF_COMMAND(DRAW, 23, 0,
{
    for (ssize_t memory_index = 0; memory_index < MEMORY_SIZE; memory_index++)
    {
        if (!memory[memory_index])
            printf(".");

        else
            printf("*");
    }

    printf("\n");
})

#undef POP_WITH_CHECK
