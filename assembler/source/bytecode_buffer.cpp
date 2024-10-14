#include <stdlib.h>
#include "assembler.h"

errors_code push_in_bytecode_buffer(assem_parametrs *assem, int number)
{
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    check_size_buffer(assem);

    assem->bytecode_buffer[assem->buffer_position] = number;
    ++assem->buffer_position;

    return ASSERT_NO_ERROR;
}

errors_code check_size_buffer(assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    if (assem->buffer_position < assem->buffer_size)
        return ASSERT_NO_ERROR;

    const int COEFFICIENT_BUFFER_INCREASE = 2;

    assem->buffer_size *= COEFFICIENT_BUFFER_INCREASE;

    assem->bytecode_buffer = (int *) realloc (assem->bytecode_buffer, (size_t) assem->buffer_size * sizeof(int));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    for (ssize_t index = assem->buffer_position; index < assem->buffer_size;index++)
        assem->bytecode_buffer[index] = 0;

    return ASSERT_NO_ERROR;
}
