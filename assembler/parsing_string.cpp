#include <string.h>
#include "assembler.h"

const char *parsing_string_to_tokens(const char *string, ssize_t *token_length, const char *separator)
{
    MYASSERT(string       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);
    MYASSERT(separator    != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);

    const char *token = strpbrk(string, separator);

    // printf("string = %s\n", string);

    if (token == NULL)
        return token;

    // printf("strspn = %lu\n", strspn(token, separator));
    token += strspn(token, separator);

    if (*token == '\0')
        return NULL;

    // printf("token = <%s>\n", token);

    if (token_length != NULL)
        *token_length = find_token_size(token, separator);

    return token;
}

ssize_t find_token_size(const char *token, const char *separator)
{
    MYASSERT(token        != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);
    MYASSERT(separator    != NULL, NULL_POINTER_PASSED_TO_FUNC, return -1);

    ssize_t token_length = 0;

    const char *pointer_end_of_token = NULL;

    if ((pointer_end_of_token = strpbrk(token, separator)) != NULL)
        token_length = pointer_end_of_token - token;

    else
        token_length = (ssize_t) strlen(token);

    return token_length;
}
