#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

// Exemple 1: $names
// Example 2: 'Stan Kyle Cartman'
// for name in $names
// Example 3: {1..5}
// Example 4: {10..0..2}
// for value in {10..0..2}
// Example 5: "${BOOKS[@]}"
// for i in {1..5}; do
// $a ${a} is the same

int has_dollar(const char *input, size_t i)
{
    return input[i] == '$';
}

int has_singleq(const char *input, size_t i, size_t end)
{
    return input[i] == '\'' && input[end] == '\'';
}

int is_int(const char *input, size_t i, size_t end)
{
    return input[i] == '{' && input[end] == '}';
}

int has_doubleq(const char *input, size_t i, size_t end)
{
    return input[i] == '\"' && input[end] == '\"';
}

size_t get_end(const char *input, size_t i)
{
    size_t j;
    for (j = i; j < strlen(input); j++)
    {
        if (input[j] == '\n' || input[j] == ';')
            break;
    }
    return j - 1;
}

size_t nb_max(size_t a)
{
    size_t tmp = a;
    size_t res = 1;
    if (a == 0)
        return 2;
    while (tmp != 0)
    {
        res += 1;
        tmp /= 10;
    }
    return res;
}

struct read_for_int* get_structure(char *input)
{
    size_t step = 1;
    size_t start = 0;
    size_t end = 0;
    size_t i = 0;
    size_t leng = strlen(input);
    /*
    * Example 1: 1..5(..1)
    * Example 2: 2..20..2
    */
    while (input[i] != '.')
    {
        start = start * 10 + ((int) input[i] - 48);
        i++;
    }
   /*
    * Example 1: ..5(..1)
    * Example 2: ..20..2
    */
    while (input[i] == '.')
        i++;
   /*
    * Example 1: 5(..1)
    * Example 2: 20..2
    */
    while (input[i] != '.')
    {
        end = end * 10 + ((int) input[i] - 48);
        i++;
    }
   /*
    * Example 1: (..1)
    * Example 2: ..2
    */
    while (input[i] == '.')
        i++;
    step = 0;
    while (i != leng)
    {
        step = step * 10 + ((int) input[i] - 48);
        i++;
    }
    struct read_for_int *res = malloc(sizeof(struct read_for_int));
    res->step = step;
    res->start = start;
    res->end = end;
    return res;
}


char *get_value(const char *input, size_t i, size_t end)
{
    char *value = malloc(((end - i) + 4) * sizeof(char));
    size_t index = 0;
   /*
    * Example 1: {1..5}
    * Example 2: {2..20..2}
    */
    while (input[i] != '{')
        i++;
   /*
    * Example 1: 1..5}
    * Example 2: 2..20..2}
    */
    i++;
    while (input[i] != '.')
    {
        value[index++] = input[i];
        i++;
    }
   /*
    * Example 1: ..5}
    * Example 2: ..20..2}
    */
    while (input[i] == '.')
        i++;
    value[index++] = '.';
    value[index++] = '.';
   /*
    * Example 1: 5}
    * Example 2: 20..2}
    */
    while (i <= end && !(input[i] == '.' || input[i] == '}'))
    {
        value[index++] = input[i];
        i++;
    }
   /*
    * Example 1: }
    * Example 2: ..2}
    */
    value[index++] = '.';
    value[index++] = '.';
    if (input[i] != '}')
    {
        while (input[i] == '.')
            i++;
        while (input[i] != '}')
        {
            value[index++] = input[i];
            i++;
        }
    }
    else
    {
        value[index++] = '1';
    }
    value[index] = '\0';
    return value;
}

// do this function after an in
size_t get_for(const char *input, size_t i, size_t *index, struct lexer *new)
{
    while (input[i] == ' ')
        i++;
    size_t nb = 0;
    size_t end = get_end(input, i);
    if (has_dollar(input, i))
    {
        new[*index - 1].current_tok = token_new(TOKEN_FOR_WORD);
        nb = end - i;
        new[*index - 1].current_tok->value = strndup(input + i, nb + 1);
        new[*index - 1].current_tok->value[nb + 1] = '\0';
    }
    else if (has_singleq(input, i, end))
    {
        new[*index - 1].current_tok = token_new(TOKEN_FOR_SINGLE_QUOTE);
        nb = end - i;
        new[*index - 1].current_tok->value = strndup(input + i + 1, nb - 1);
        new[*index - 1].current_tok->value[nb - 1] = '\0';
    }
    else if (has_doubleq(input, i, end))
    {
        new[*index - 1].current_tok = token_new(TOKEN_FOR_DOUBLE_QUOTE);
        nb = end - i;
        new[*index - 1].current_tok->value = strndup(input + i + 1, nb - 1);
        new[*index - 1].current_tok->value[nb - 1] = '\0';
    }
    else if (is_int(input, i, end))
    {
        new[*index - 1].current_tok = token_new(TOKEN_FOR_INT);
        nb = end - i;
        new[*index - 1].current_tok->value = get_value(input, i, end);
    }
    else
    {
        new[*index - 1].current_tok = token_new(TOKEN_FOR_WORD);
        nb = end - i;
        new[*index - 1].current_tok->value = strndup(input + i, nb + 2);
        new[*index - 1].current_tok->value[nb + 1] = '\0';   
    }
    return nb;
}
