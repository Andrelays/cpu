#include <stdlib.h>
#include "assembler.h"

errors_code assem_parametrs_destructor(assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->source_code         != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    assem->buffer_position = -1;
    assem->buffer_size     = -1;

    text_parametrs_destructor(assem->source_code);

    free(assem->source_code);
    free(assem->bytecode_buffer);

    assem->bytecode_buffer = NULL;
    assem = NULL;

    return ASSERT_NO_ERROR;
}
