#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdio.h>
#include <SDL2/SDL.h>
#include "../enum.h"
#include "../libraries/Stack/stack.h"
#include "../libraries/Stack/myassert.h"

#define POP_WITH_CHECK(stk, pop_value)                              \
 do {                                                               \
    if(pop(stk, pop_value) != NO_ERROR)                             \
    {                                                               \
        printf(RED "ERROR! Pop from an emptystack\n" RESET_COLOR);  \
        return INVALID_OPERATOR;                                    \
    }                                                               \
                                                                    \
}  while(0)

const ssize_t MEMORY_SIZE     = 3000;
const ssize_t LINE_WIDTH      = 100;
const ssize_t COLUMN_WIDTH    = MEMORY_SIZE / LINE_WIDTH;
const int     SQUARE_SIZE     = 20;
const int     SCREEN_WIDTH    = LINE_WIDTH   * SQUARE_SIZE;
const int     SCREEN_HEIGHT   = COLUMN_WIDTH * SQUARE_SIZE;
const size_t  FRAME_DELAY     = 30e3;

struct processor_parametrs {
    int                 *regs;
    int                 *memory;
    int                 *buffer;
    stack               *stk;
    SDL_Renderer        *renderer;
    SDL_Window          *window;
    size_t               buffer_size;
    size_t               buffer_position;
};

errors_code processor(FILE *byte_code_file_pointer, FILE *logs_file_pointer);
errors_code processor_parametrs_constructor(FILE *byte_code_file_pointer, processor_parametrs *processor_info);
errors_code processor_parametrs_destructor(processor_parametrs *processor_info);
int pop_from_bytecode_buffer(processor_parametrs *processor_info);
bool check_args(int code_operator, processor_parametrs *processor_info, int **pointer_to_arg);

#endif //ASSEMBLER_H_INCLUDED
