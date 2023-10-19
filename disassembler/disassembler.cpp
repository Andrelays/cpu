#include "disassembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"
#include <math.h>
#include <string.h>

FILE *Global_input_pointer  = NULL;
FILE *Global_output_pointer = NULL;

// #define COMMAND_LIST\
//     DO(HLT)\
//     DO(PUSH)\
//     DO(SUB)\
//     DO(DIV)\
//     DO(IN)\
//     DO(OUT)\
//     DO(MUL)\
//     DO(ADD)\
//     DO(SQRT)\
//     DO(SIN)\
//     DO(COS)\
//     DO(POP)
//
// #define DO(COMMAND)\
//     printf("%s\n", COMMAND.name);
//     COMMAND_LIST
// #undef


ssize_t disassembler(struct text_parametrs *byte_code)
{
    MYASSERT(Global_input_pointer  != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(Global_output_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC , return NULL_POINTER_PASSED_TO_FUNC);

    int code_operator = NO_OPERATOR;

    char *buffer = strtok(byte_code->buffer, " \n\t\r");

    while(buffer != NULL && sscanf(buffer, "%d", &code_operator))
    {
        switch (code_operator)
        {

            #define CHECK_OPERATOR_(command)                                                         \
                case command:                                                                        \
                    fprintf(Global_output_pointer, "%s ", #command);                                 \
                    break;

            CHECK_OPERATOR_(HLT);
            CHECK_OPERATOR_(PUSH);
            CHECK_OPERATOR_(SUB);
            CHECK_OPERATOR_(DIV);
            CHECK_OPERATOR_(IN);
            CHECK_OPERATOR_(OUT);
            CHECK_OPERATOR_(MUL);
            CHECK_OPERATOR_(ADD);
            CHECK_OPERATOR_(SQRT);
            CHECK_OPERATOR_(SIN);
            CHECK_OPERATOR_(COS);
            CHECK_OPERATOR_(POP);

            #undef CHECK_OPERATOR_

            default:
                printf("ERROR! Incorrect command: \"%d\"\n", code_operator);
                return INVALID_COMMAND;
        }

        printf("%s ", buffer);

        buffer = strtok(NULL, " \n\t\r");

        if (buffer != NULL && code_operator == PUSH)
        {
            fprintf(Global_output_pointer, "%s", buffer);
            buffer = strtok(NULL, " \n\t\r");
        }

        fputc('\n', Global_output_pointer);

        code_operator = NO_OPERATOR;
    }

    return NO_ERROR;
}

// int *save_data_to_array(FILE *file_pointer)
// {
//     size_t size_file = determine_size(file_pointer);
//
//     int *buffer = (int *)calloc(size_file + 1, sizeof(int));
//     MYASSERT(buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);
//
//     size_file = fread(buffer, sizeof(int), size_file, file_pointer);
//
//     buffer[size_file] = '\0';
//
//     return buffer;
// }
