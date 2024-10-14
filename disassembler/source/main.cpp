#include "disassembler.h"
#include "myassert.h"
#include "onegin.h"
#include "stack.h"

int main(int argc, const char *argv[])
{
    const int CORRECT_NUMBER_ARGC = 3;

    if(!check_argc(argc, CORRECT_NUMBER_ARGC))
        return -1;

    const char *file_name_input     = argv[1];
    const char *file_name_output    = argv[2];

    FILE *byte_code_file_pointer   = check_isopen_old(file_name_input,  "rb");
    FILE *source_code_file_pointer = check_isopen_old(file_name_output, "w");

    MYASSERT(byte_code_file_pointer   != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(source_code_file_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    disassembler(byte_code_file_pointer, source_code_file_pointer);

    MYASSERT(check_isclose (byte_code_file_pointer),    COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (source_code_file_pointer),  COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
}
