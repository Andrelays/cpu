#include "cpu.h"
#include "../libraries/Stack/myassert.h"
#include "../libraries/Onegin/onegin.h"
#include "../libraries/Stack/stack.h"

int main(int argc, const char *argv[])
{
    const int correct_number_argc = 3;

    if(!check_argc(argc, correct_number_argc))
        return -1;

    const char *file_name_input  = argv[1];
    const char *file_name_logs   = argv[2];

    FILE *byte_code_file_pointer = check_isopen_old(file_name_input, "rb");
    FILE *logs_file_pointer      = check_isopen(file_name_logs, "w");

    MYASSERT(byte_code_file_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(logs_file_pointer      != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    Global_color_output = false;

    processor(byte_code_file_pointer, logs_file_pointer);

    MYASSERT(check_isclose (byte_code_file_pointer), COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (logs_file_pointer),      COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);

    return 0;
}

