#include <stdlib.h>
#include "assembler.h"

errors_code assem_parametrs_constructor(assem_parametrs *assem, FILE *source_code_file_pointer)
{
    MYASSERT(assem                    != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_code_file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);


    assem->buffer_position       = 0;
    assem->labels_index_position = 0;
    assem->buffer_size           = INITIAL_BUFFER_SIZE;

    assem->bytecode_buffer = (int *) calloc(INITIAL_BUFFER_SIZE, sizeof(int));
    struct text_parametrs *source_code = (text_parametrs *) calloc(1, sizeof(text_parametrs));

    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);
    MYASSERT(assem->bytecode_buffer != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return FAILED_TO_ALLOCATE_DYNAM_MEMOR);

    text_parametrs_constructor(source_code, source_code_file_pointer);
    replace_newline_char_and_comments(source_code->buffer);

    assem->source_code = source_code;

    return ASSERT_NO_ERROR;
}

errors_code replace_newline_char_and_comments(char *text)
{
    MYASSERT(text != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    while (*text != '\0')
    {
        if (*text == '\r' || *text == '\n' || *text == ';')
            *text = '\0';

        ++text;
    }

    return ASSERT_NO_ERROR;
}
