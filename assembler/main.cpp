#include "assembler.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"

int main(int argc, const char *argv[])
{
    const int CORRECT_NUMBER_ARGC = 4;

    struct assem_parametrs assem = {
        .bytecode_buffer     = NULL,
        .buffer_size         = 0,
        .buffer_position     = 0
    };

    if(!check_argc(argc, CORRECT_NUMBER_ARGC))
        return -1;

    const char *file_name_input     = argv[1];
    const char *file_name_output    = argv[2];
    const char *file_name_listening = argv[3];

    FILE *source_code_file_pointer = check_isopen_old(file_name_input,     "r");
    FILE *byte_code_file_pointer   = check_isopen_old(file_name_output,    "wb"); //TODO BIN
    FILE *listening_file_pointer   = check_isopen_old(file_name_listening, "w");

    MYASSERT(source_code_file_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(byte_code_file_pointer   != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(listening_file_pointer   != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    assem_parametrs_constructor(&assem);

    assembler(&assem, source_code_file_pointer, byte_code_file_pointer, listening_file_pointer);
    assembler(&assem, source_code_file_pointer, byte_code_file_pointer, listening_file_pointer);

    assem_parametrs_destructor(&assem);

    MYASSERT(check_isclose (source_code_file_pointer),  COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (byte_code_file_pointer),    COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (listening_file_pointer),    COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
}
