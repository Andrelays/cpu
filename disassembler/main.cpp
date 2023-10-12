#include "disassembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"

int main(int argc, const char *argv[])
{
    const int correct_number_argc = 3;

    if(!check_argc(argc, correct_number_argc))
        return -1;

    struct text_parametrs source_code = {
        .string_array = NULL,
        .buffer       = NULL,
        .number_lines = 0
    };

    const char *file_name_input  = argv[1];
    const char *file_name_output = argv[2];

    Global_input_pointer  = check_isopen_old(file_name_input,  "r");
    Global_output_pointer = check_isopen_old(file_name_output, "w");

    constructor(&source_code, Global_input_pointer);

    disassembler(&source_code);

    destructor(&source_code);

    check_isclose(Global_input_pointer);
    check_isclose(Global_output_pointer);
}
