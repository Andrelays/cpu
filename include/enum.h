#ifndef ENUM_H_INCLUDED
#define ENUM_H_INCLUDED

#include <stdio.h>

const ssize_t NUMBER_OF_REGISTERS = 4;
const int     DEGREE_ACCURACY     = 100;

#define DEF_COMMAND(command, id, number_args, ...) command = id,

enum command {
    #include "commands.h"
    NO_OPERATOR    = 0
};

#undef DEF_COMMAND

enum command_args {
    COMMAND_ARGS_REGISTER            = 0b010'00000,
    COMMAND_ARGS_IMMEDIATE           = 0b001'00000,
    COMMAND_ARGS_MEMORY              = 0b100'00000,
    COMMAND_ARGS_MEMORY_NUMBER       = 0b101'00000,
    COMMAND_ARGS_MEMORY_REGISTER     = 0b110'00000,
    COMMAND_ARGS_ALL                 = 0b111'00000
};

#endif //ENUM_H_INCLUDED
