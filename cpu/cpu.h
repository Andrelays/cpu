#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../processor.h"
#include "../libraries/Stack/stack.h"
#include <stdio.h>

extern FILE *Global_input_pointer;
extern int Global_degree_accuracy;

ssize_t processor(stack *stk);

#endif //ASSEMBLER_H_INCLUDED
