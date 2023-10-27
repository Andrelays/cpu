#include <string.h>
#include "assembler.h"

ssize_t find_label(char *label, assem_parametrs *assem)
{
    MYASSERT(assem                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(assem->bytecode_buffer     != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(label                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);

    label_parametrs *labels = assem->labels_array;

    for (ssize_t label_index = 0; label_index < assem->labels_index_position; label_index++)
    {
        if (strcmp((labels[label_index]).label_name, label) == 0)
            return (labels[label_index]).label_position;
    }

    return -1;
}

bool check_is_label(char *string, assem_parametrs *assem)
{
    MYASSERT(string != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);
    MYASSERT(assem  != NULL, NULL_POINTER_PASSED_TO_FUNC, return false);

    ssize_t label_len = 0;
    label_parametrs *labels = assem->labels_array;

    label_len = find_token_size(string, " \r\n\t");

    MYASSERT(label_len > 0, NEGATIVE_VALUE_SIZE_T, return false);

    if (parsing_string_to_tokens(string, NULL, " \r\n\t"))
        return false;

    if (string[label_len - 1] == ':')
    {
        if (assem->labels_index_position >= SIZE_ARRAY_LABELS)
        {
            printf(RED "ERROR: Too many labels, max: %ld\n" RESET_COLOR, SIZE_ARRAY_LABELS);
            return false;
        }

        (labels[assem->labels_index_position]).label_position = assem->buffer_position;

        strncpy((labels[assem->labels_index_position]).label_name, string, (size_t) label_len - 1);

        ++assem->labels_index_position;

        return true;
    }

    return false;
}
