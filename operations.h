#include <stdio.h>

enum command {
    INVALID_OPERATOR    = -2,
    HLT                 =  1,
    PUSH                =  2,
    SUB                 =  3,
    DIV                 =  4,
    IN                  =  5,
    OUT                 =  6,
    MUL                 =  7,
    ADD                 =  8,
    SQRT                =  9,
    SIN                 =  10,
    COS                 =  11,
    POP                 =  12
};

enum command_args {
    COMMAND_ARGS_REGISTER            = 0b010'00000,
    COMMAND_ARGS_NUMBER              = 0b001'00000,
    COMMAND_ARGS_REGISTER_AND_NUMBER = 0b011'00000
};

// struct operation {
//     const char*         name;
//     ssize_t      number_args;
//     short                 id;
//     ssize_t             size;
// };
//
// #define DEF_COMMAND(command, id, number_args, ...) {#command, number_args, command, sizeof #command - 1},
//
// const operation OPERATIONS[] =
// {
//     DEF_COMMAND(HLT,   1, 0)
//     DEF_COMMAND(PUSH,  2, 1)
//     DEF_COMMAND(SUB,   3, 0)
//     DEF_COMMAND(DIV,   4, 0)
//     DEF_COMMAND(IN,    5, 0)
//     DEF_COMMAND(OUT,   6, 0)
//     DEF_COMMAND(MUL,   7, 0)
//     DEF_COMMAND(ADD,   8, 0)
//     DEF_COMMAND(SQRT,  9, 0)
//     DEF_COMMAND(SIN,  10, 0)
//     DEF_COMMAND(COS,  11, 0)
//     DEF_COMMAND(POP,  12, 1)
// };
//
// #undef DEF_COMMAND

// nri-----
// 00000000
//
// n - пока ничего
// r - регистр
// i - immed (push 5 5 - immed)
// ----- -
