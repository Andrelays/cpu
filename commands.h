DEF_COMMAND(HLT,   1, 0,
{
    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    return NO_ERROR;
})


DEF_COMMAND(PUSH,  2, 1,
{
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
})

DEF_COMMAND(SUB,   3, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, pop_value_1 - pop_value_2);
})

DEF_COMMAND(DIV,   4, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, DEGREE_ACCURACY * pop_value_1 / pop_value_2);
})

DEF_COMMAND(IN,    5, 0,
{
    scanf("%d", &push_value);
    push(stk, push_value * DEGREE_ACCURACY);
})

DEF_COMMAND(OUT,   6, 0,
{
    pop(stk, &pop_value_1);
    printf("%g\n", (float) pop_value_1 / ((float) DEGREE_ACCURACY));
})

DEF_COMMAND(MUL,   7, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, pop_value_1 * pop_value_2 / DEGREE_ACCURACY);
})

DEF_COMMAND(ADD,   8, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, pop_value_1 + pop_value_2);
})

DEF_COMMAND(SQRT,  9, 0,
{
    pop(stk, &pop_value_1);
    push(stk, (int) sqrt(pop_value_1 * DEGREE_ACCURACY));
})

DEF_COMMAND(SIN,  10, 0,
{
    pop(stk, &pop_value_1);
    push(stk, (int) (sin((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(COS,  11, 0,
{
    pop(stk, &pop_value_1);
    push(stk, (int) (cos((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(POP,  12, 1,
{
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
})

DEF_COMMAND(JMP,  13, 1,
{
    bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];
})

DEF_COMMAND(JA,  14, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 > pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JAE,  15, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 >= pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JB,  16, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 < pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JBE,  17, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 <= pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JE,  18, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 == pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JNE,  19, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 != pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})


