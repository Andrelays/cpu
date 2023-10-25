DEF_COMMAND(HLT,  -1, 0,
{
    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    return ASSERT_NO_ERROR;
})


DEF_COMMAND(PUSH,  2, 1,
{
    // MASK_LOWER(3) -> 0b0000'0111
    // MASK_LOWER(N) ((1u << (N)) - 1)

    int mem_reg = ((code_operator >> 5) & 0x7); //TODO switch
    switch(mem_reg)
    if (code_operator & COMMAND_ARGS_MEMORY_REGISTER)
    {
        int reg = pop_from_bytecode_buffer(&bytecode_info);

        if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
            push_value = regs[reg - 1];

        else
        {
            printf("ERROR: INCORRECT NUMBER REG IN PUSH\n");
            return INVALID_OPERATOR;
        }
    }

    else if (code_operator & COMMAND_ARGS_REGISTER)
    {
        int reg = pop_from_bytecode_buffer(&bytecode_info);

        if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
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

    push(stk, DEGREE_ACCURACY * pop_value_1 / pop_value_2); //TODO chaeck zero val 2
})

DEF_COMMAND(IN,    5, 0,
{
    scanf("%d", &push_value);
    push(stk, push_value * DEGREE_ACCURACY);
})

DEF_COMMAND(OUTC, 6, 0,
{
    pop(stk, &pop_value_1);
    printf("%c", pop_value_1 / DEGREE_ACCURACY);
})

DEF_COMMAND(OUT,   7, 0,
{
    pop(stk, &pop_value_1);
    printf("%g\n", (float) pop_value_1 / ((float) DEGREE_ACCURACY));
})

DEF_COMMAND(MUL,   8, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, pop_value_1 * pop_value_2 / DEGREE_ACCURACY); //TODO check overflow
})

DEF_COMMAND(ADD,   9, 0,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    push(stk, pop_value_1 + pop_value_2);
})

DEF_COMMAND(SQRT,  10, 0,
{
    pop(stk, &pop_value_1); // TODO: check that pop did not fail
    push(stk, (int) sqrt(pop_value_1 * DEGREE_ACCURACY));
})

DEF_COMMAND(SIN,  11, 0,
{
    pop(stk, &pop_value_1);
    push(stk, (int) (sin((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(COS,  12, 0,
{
    pop(stk, &pop_value_1);
    push(stk, (int) (cos((float) pop_value_1 / (float) DEGREE_ACCURACY) * (float) DEGREE_ACCURACY));
})

DEF_COMMAND(POP,  13, 1,
{
    pop(stk, &pop_value_1);

    if (code_operator & COMMAND_ARGS_REGISTER)
    {
        int reg = pop_from_bytecode_buffer(&bytecode_info);

        if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
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

DEF_COMMAND(JMP,  14, 1,
{
    bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];
})

DEF_COMMAND(JA,  15, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 > pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JAE,  16, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 >= pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position; // TODO: define JUMP(>=)
})

DEF_COMMAND(JB,  17, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 < pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JBE,  18, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 <= pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JE,  19, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 == pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(JNE,  20, 1,
{
    pop(stk, &pop_value_2);
    pop(stk, &pop_value_1);

    if (pop_value_1 != pop_value_2)
        bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];

    else
        ++bytecode_info.buffer_position;
})

DEF_COMMAND(CALL,  21, 1,
{
    push(stk, (int) bytecode_info.buffer_position + 1);
    bytecode_info.buffer_position = bytecode_info.buffer[bytecode_info.buffer_position];
})

DEF_COMMAND(RETURN, 22, 0,
{
    pop(stk, &pop_value_1);
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
