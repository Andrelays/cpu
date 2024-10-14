

DEF_COMMAND(HLT,  31, 0,
{
    processor_parametrs_destructor(&processor_info);

    return ASSERT_NO_ERROR;
})

DEF_COMMAND(PUSH,  2, 1,
{
    push(stk, *pointer_to_arg);
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

    push(stk, pop_value_1 * pop_value_2 / DEGREE_ACCURACY);
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

    if (pointer_to_arg == NULL)
    {
        printf(RED "ERROR: INCORRECT ARG IN POP\n" RESET_COLOR);
        return INVALID_OPERATOR;
    }

    *pointer_to_arg = pop_value_1;
})

DEF_COMMAND(JMP,  14, 1,
{
    processor_info.buffer_position = (size_t) *pointer_to_arg / DEGREE_ACCURACY;
})

#define JUMP_WITH_CONDITION(name, id, sign)                                                                 \
DEF_COMMAND(name, id, 1,                                                                                    \
{                                                                                                           \
    POP_WITH_CHECK(stk, &pop_value_2);                                                                      \
    POP_WITH_CHECK(stk, &pop_value_1);                                                                      \
                                                                                                            \
    if (pop_value_1 sign pop_value_2) {                                                                     \
        processor_info.buffer_position = (size_t) *pointer_to_arg / DEGREE_ACCURACY;                        \
    }                                                                                                       \
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
    push(stk, (int) processor_info.buffer_position);
    processor_info.buffer_position = (size_t) *pointer_to_arg / DEGREE_ACCURACY;
})

DEF_COMMAND(RETURN, 22, 0,
{
    POP_WITH_CHECK(stk, &pop_value_1);
    processor_info.buffer_position = (size_t) pop_value_1;
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
    SDL_SetRenderDrawColor(processor_info.renderer, 255, 255, 255, 255);

    for (size_t line_number = 0; line_number < COLUMN_WIDTH; line_number++)
    {
        for (size_t column_number = 0; column_number < LINE_WIDTH; column_number++)
        {
            if (memory[column_number + line_number * LINE_WIDTH])
            {
                SDL_Rect rect1 = {(int) column_number * SQUARE_SIZE, (int) line_number * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderFillRect(processor_info.renderer, &rect1);
            }
        }
    }

    static SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT)
        {
            processor_parametrs_destructor(&processor_info);
            exit(0);

            return ASSERT_NO_ERROR;
        }
    }


    SDL_RenderPresent(processor_info.renderer);

    SDL_SetRenderDrawColor(processor_info.renderer, 0, 0, 0, 0);
    usleep(FRAME_DELAY);

    SDL_RenderClear(processor_info.renderer);
})

DEF_COMMAND(INIT_DRAW, 25, 0,
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &(processor_info.window), &(processor_info.renderer));
    SDL_SetRenderDrawColor(processor_info.renderer, 0, 0, 0, 0);
    SDL_RenderClear(processor_info.renderer);
})

#undef POP_WITH_CHECK
