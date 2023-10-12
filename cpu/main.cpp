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

    Global_input_pointer  = check_isopen(file_name_input,  "r");
    Global_logs_pointer   = check_isopen(file_name_logs,   "w");

    MYASSERT(Global_input_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(Global_logs_pointer  != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    Global_color_output = false;

    stack *stk = get_pointer_stack();

    STACK_CONSTRUCTOR(stk);

    processor(stk);

    stack_destructor(stk);

    MYASSERT(check_isclose (Global_logs_pointer),  COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (Global_input_pointer), COULD_NOT_CLOSE_THE_FILE, return COULD_NOT_CLOSE_THE_FILE);

    return 0;
}
