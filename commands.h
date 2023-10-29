//WARNING HLT 31st command!!!

#define POP_WITH_CHECK(stk, pop_value)                              \
 do {                                                               \
    if(pop(stk, pop_value) != NO_ERROR)                             \
    {                                                               \
        printf(RED "ERROR! Pop from an emptystack\n" RESET_COLOR);  \
        return INVALID_OPERATOR;                                    \
    }                                                               \
                                                                    \
}  while(0)

DEF_COMMAND(HLT,  -1, 0,
{
    stack_destructor(stk);
    bytecode_parametrs_destructor(&bytecode_info);

    free(memory);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return ASSERT_NO_ERROR;
})

DEF_COMMAND(PUSH,  2, 1,
{
    switch(code_operator & COMMAND_ARGS_ALL)
    {
        case(COMMAND_ARGS_MEMORY_REGISTER):
        {
            //usleep(2e5);

            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
            {
                if((number_memory_cell = regs[reg - 1] / DEGREE_ACCURACY) < MEMORY_SIZE && number_memory_cell >= 0) {
                    push_value = memory[number_memory_cell];
                }

                else {
                    printf(RED "ERROR: INCORRECT NUMBER MEMORY CELL IN PUSH\n" RESET_COLOR);
                    return INVALID_OPERATOR;
                }
            }

            else {
                printf(RED "ERROR: INCORRECT NUMBER REG IN PUSH\n" RESET_COLOR);
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_NUMBER):
        {
            //usleep(2e5);

            number_memory_cell = pop_from_bytecode_buffer(&bytecode_info);

            if(number_memory_cell < MEMORY_SIZE && number_memory_cell >= 0) {
                push_value = memory[number_memory_cell];
            }

            else {
                printf(RED "ERROR: INCORRECT NUMBER MEMORY CELL IN PUSH\n" RESET_COLOR);
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
                printf(RED "ERROR: INCORRECT NUMBER REG IN PUSH\n" RESET_COLOR);
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_NUMBER):
        {
            push_value = pop_from_bytecode_buffer(&bytecode_info) * DEGREE_ACCURACY;

            break;
        }

        default:
        {
            printf(RED "ERROR: INCORRECT NUMBER ARGS OF PUSH\n" RESET_COLOR);
            return INVALID_OPERATOR;
        }
    }

    push(stk, push_value);
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
        printf(RED "ERROR! ATTEMPT TO DIV BY 0\n" RESET_COLOR);
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
                regs[reg - 1] = pop_value_1;
            }

            else {
                printf(RED "ERROR: INCORRECT NUMBER REG IN POP\n" RESET_COLOR);
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_REGISTER):
        {
            //usleep(2e5);

            int reg = pop_from_bytecode_buffer(&bytecode_info);

            if (1 <= reg && reg <= NUMBER_OF_REGISTERS)
            {
                if(((number_memory_cell = (regs[reg - 1]) / DEGREE_ACCURACY)) < MEMORY_SIZE && number_memory_cell >= 0) {
                    memory[number_memory_cell] = pop_value_1;
                }

                else {
                    printf(RED "ERROR: INCORRECT NUMBER MEMORY CELL IN POP\n" RESET_COLOR);
                    return INVALID_OPERATOR;
                }
            }

            else {
                printf(RED "ERROR: INCORRECT NUMBER REG IN POP\n" RESET_COLOR);
                return INVALID_OPERATOR;
            }

            break;
        }

        case(COMMAND_ARGS_MEMORY_NUMBER):
        {
            //usleep(2e5);

            number_memory_cell = pop_from_bytecode_buffer(&bytecode_info);

            if(number_memory_cell < MEMORY_SIZE && number_memory_cell >= 0) {
                memory[number_memory_cell] = pop_value_1;
            }

            else {
                printf(RED "ERROR: INCORRECT NUMBER REG IN POP\n" RESET_COLOR);
                return INVALID_OPERATOR;
            }

            break;
        }

        default:
        {
            printf(RED "ERROR: INCORRECT NUMBER ARGS OF POP\n" RESET_COLOR);
            return INVALID_OPERATOR;
        }
    }
})

DEF_COMMAND(JMP,  14, 1,
{
    bytecode_info.buffer_position = (size_t) bytecode_info.buffer[bytecode_info.buffer_position];
})

#define JUMP_WITH_CONDITION(name, id, sign)                                                             \
DEF_COMMAND(name, id, 1,                                                                                \
{                                                                                                       \
    POP_WITH_CHECK(stk, &pop_value_2);                                                                  \
    POP_WITH_CHECK(stk, &pop_value_1);                                                                  \
                                                                                                        \
    if (pop_value_1 sign pop_value_2) {                                                                 \
        bytecode_info.buffer_position = (size_t) bytecode_info.buffer[bytecode_info.buffer_position];   \
    }                                                                                                   \
                                                                                                        \
    else {                                                                                              \
        ++bytecode_info.buffer_position;                                                                \
    }                                                                                                   \
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
    bytecode_info.buffer_position = (size_t) bytecode_info.buffer[bytecode_info.buffer_position];
})

DEF_COMMAND(RETURN, 22, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    bytecode_info.buffer_position = (size_t) pop_value_1;
})

DEF_COMMAND(DRAW, 23, 0,
{
    for (size_t line_number = 0; line_number < COLUMN_WIDTH; line_number++)
    {
        for (size_t column_number = 0; column_number < LINE_WIDTH; column_number++)
        {
            if (!memory[column_number + line_number * LINE_WIDTH])
                printf(".");

            else
                printf("*");
        }

        printf("\n");
    }

    printf("\n");
})

DEF_COMMAND(DRAWF, 24, 0,
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t line_number = 0; line_number < COLUMN_WIDTH; line_number++)
    {
        for (size_t column_number = 0; column_number < LINE_WIDTH; column_number++)
        {
            if (memory[column_number + line_number * LINE_WIDTH])
            {
                SDL_Rect rect1 = {(int) column_number * SQUARE_SIZE, (int) line_number * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderFillRect(renderer, &rect1);
            }
        }
    }

    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    usleep(82e3);

    SDL_RenderClear(renderer);
})

DEF_COMMAND(INIT_DRAW, 25, 0,
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
})

#undef POP_WITH_CHECK
