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
// for **iin {1..5}; do
// $a ${a} is the same

int has_dollar(const char *input, size_t *i)
{
    return input[*i] == '$';
}

int has_singleq(const char *input, size_t *i, size_t end)
{
    return input[*i] == '\'' && input[end] == '\'';
}

int is_int(const char *input, size_t *i, size_t end)
{
    return input[*i] == '{' && input[end] == '}';
}

int has_doubleq(const char *input, size_t *i, size_t end)
{
    return input[*i] == '\"' && input[end] == '\"';
}

size_t get_end(const char *input, size_t *i)
{
    size_t j;
    int single_q = (input[*i] == '\'');
    int double_q = (input[*i] == '\"');
    int is_nb = (input[*i] == '{');
    int special = single_q || double_q || is_nb;
    for (j = *i + 1; j < strlen(input); j++)
    {
        if (input[j] == '\n' || input[j] == ';')
            break;
        if (!special && input[j] == ' ')
            break;
        else if ((single_q && input[j] == '\'')
                || (double_q && input[j] == '\"')
                || (is_nb && input[j] == '}'))
                break;
    }
    if (!special)
        j--;
    return j;
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
    while (i!= leng)
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


char *get_value(const char *input, struct cap_and_i *s, size_t end)
{
    char *value = malloc(((end - *s->i) + 4) * sizeof(char));
    size_t index = 0;
   /*
    * Example 1: {1..5}
    * Example 2: {2..20..2}
    */
    while (input[*s->i] == '{')
        *s->i = *s->i + 1;
   /*
    * Example 1: 1..5}
    * Example 2: 2..20..2}
    */
    while (input[*s->i] != '.')
    {
        value[index++] = input[*s->i];
        *s->i = *s->i + 1;
    }
   /*
    * Example 1: ..5}
    * Example 2: ..20..2}
    */
    while (input[*s->i] == '.')
        *s->i = *s->i + 1;
    value[index++] = '.';
    value[index++] = '.';
   /*
    * Example 1: 5}
    * Example 2: 20..2}
    */
    while (index <= end && !(input[*s->i] == '.' || input[*s->i] == '}'))
    {
        value[index++] = input[*s->i];
        *s->i = *s->i + 1;
    }
   /*
    * Example 1: }
    * Example 2: ..2}
    */
    value[index++] = '.';
    value[index++] = '.';
    if (input[*s->i] != '}')
    {
        while (input[*s->i] == '.')
            *s->i = *s->i + 1;
        while (input[*s->i] != '}')
        {
            value[index++] = input[*s->i];
            *s->i = *s->i + 1;
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
void get_for(const char *input, struct cap_and_i *s, size_t *index, struct lexer *new)
{
    while (*s->i < strlen(input) && input[*s->i] != ';')
    {
        if (*s->cap < (*index + 2))
        {
          *s->cap *= 2;
          new = realloc(new, *s->cap * sizeof(struct lexer));
        }
        while (input[*s->i] == ' ')
            *s->i = *s->i + 1;
        size_t nb = 0;
        size_t end = get_end(input, s->i);
        if (has_dollar(input, s->i))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR_WORD);
            nb = end - *s->i;
            new[*index - 1].current_tok->value = strndup(input + *s->i, nb + 1);
            new[*index - 1].current_tok->value[nb + 1] = '\0';
            *s->i = *s->i + nb;
        }
        else if (has_singleq(input, s->i, end))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR_SINGLE_QUOTE);
            nb = end - *s->i;
            new[*index - 1].current_tok->value = strndup(input + *s->i + 1, nb - 1);
            new[*index - 1].current_tok->value[nb - 1] = '\0';
            *s->i = *s->i + nb;
        }
        else if (has_doubleq(input, s->i, end))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR_DOUBLE_QUOTE);
            nb = end - *s->i;
            new[*index - 1].current_tok->value = strndup(input + *s->i + 1, nb - 1);
            new[*index - 1].current_tok->value[nb - 1] = '\0';
            *s->i = *s->i + nb;
        }
        else if (is_int(input, s->i, end))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR_INT);
            //nb = end - *s->i;
            new[*index - 1].current_tok->value = get_value(input, s, end);
        }
        else
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR_WORD);
            nb = end - *s->i;
            new[*index - 1].current_tok->value = strndup(input + *s->i, nb + 2);
            new[*index - 1].current_tok->value[nb + 1] = '\0';
            *s->i = *s->i + nb; 
        }
        *index = *index + 1;
        *s->i = *s->i + 1;
    }
}
