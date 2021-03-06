#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct info_lexer *lexer_init()
{
    struct info_lexer *new = malloc(sizeof(struct info_lexer));
    new->cap = malloc(sizeof(size_t));
    *new->cap = 20;
    new->lexer = malloc(20 * sizeof(struct lexer));
    new->index = malloc(sizeof(size_t));
    *new->index = 1;
    return new;
}

void lexer_info_free(struct info_lexer *info)
{
    if (info->cap)
        free(info->cap);
    if (info->index)
        free(info->index);
    if (info->lexer)
        lexer_free(info->lexer);
    if (info)
        free(info);
}

void clear_info(struct info_lexer *new)
{
    while (new->lexer->current_tok->type != TOKEN_EOF)
    {
        struct token *token = lexer_pop(new->lexer);
        while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
        {
            token_free(token);
            token = lexer_pop(new->lexer);
        }
        token_free(token);
    }

    /*struct token *token = lexer_pop(new->lexer);
    while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
    {
        token_free(token);
        token = lexer_pop(new->lexer);
    }
    token_free(token);*/
    *new->index = 1;
}

/*
 *
 *
 *  Is a variable
 *  return 0 if just variable
 *  return 1 if end is single quote
 *  return 2 if end is double quote
 *
 */

int var2(const char *command, size_t index)
{
    for (size_t i = index; i < strlen(command) && command[i] != ' '; i++)
    {
        if (command[i] == '=')
        {
            size_t j = i + 1; 
            while ( j < strlen(command) && command[j] == ' ')
            {
                j++;
            }
            if (command[j] == '\'')
            {
                return 1;
            }
            if (command[j] == '\"')
            {
                return 2;
            }
            return 0;
        }
    }
    return -1;
}

struct cap_and_i *spe_token(const char *input, size_t *index,
                            struct cap_and_i *for_s, struct lexer *new)
{
    size_t nb = 0;
    size_t *i = for_s->i;
    size_t len = strlen(input);
    char current_char = input[*i];
    if (current_char >= 'A' && current_char <= 'Z')
        current_char = current_char + ('a' - 'A');
    if (current_char >= 'a' && current_char <= 'z')
    {
        // printf("char is right now %c \n", current_char);
        /*
         * testing echo to change mode
         */
        if (current_char == 'e' && *for_s->i + 3 < len
            && input[*for_s->i + 1] == 'c' && input[*for_s->i + 2] == 'h'
            && input[*for_s->i + 3] == 'o'
            && (*for_s->i + 4 == len
                || (*for_s->i + 4 < len
                    && (input[*for_s->i + 4] == ' '
                        || input[*for_s->i + 4] == ';'
                        || input[*for_s->i + 4] == ')'
                        || input[*for_s->i + 4] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_WORDS);
            new[*index - 1].current_tok->value =
                strndup("echo", strlen("echo"));
            for_s->mode = 1;
            nb = 3;
        }
        /*
         * testing if
         */
        else if (for_s->mode == 0 && current_char == 'i' && *for_s->i + 1 < len
                 && input[*for_s->i + 1] == 'f'
                 && (*for_s->i + 2 == len
                     || (*for_s->i + 2 < len
                         && (input[*for_s->i + 2] == ' '
                             || input[*for_s->i + 2] == ';'
                             || input[*for_s->i + 2] == ')'
                             || input[*for_s->i + 2] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_IF);
            new[*index - 1].current_tok->value = NULL;
            nb = 1;
        }
        /*
         * testing else
         */
        else if (for_s->mode == 0 && current_char == 'e' && *for_s->i + 3 < len
                 && input[*for_s->i + 1] == 'l' && input[*for_s->i + 2] == 's'
                 && input[*for_s->i + 3] == 'e'
                 && (*for_s->i + 4 == len
                     || (*for_s->i + 4 < len
                         && (input[*for_s->i + 4] == ' '
                             || input[*for_s->i + 4] == ';'
                             || input[*for_s->i + 4] == ')'
                             || input[*for_s->i + 4] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_ELSE);
            new[*index - 1].current_tok->value = NULL;
            nb = 3;
        }
        /*
         * testing elif
         */
        else if (for_s->mode == 0 && current_char == 'e' && *for_s->i + 3 < len
                 && input[*for_s->i + 1] == 'l' && input[*for_s->i + 2] == 'i'
                 && input[*for_s->i + 3] == 'f'
                 && (*for_s->i + 4 == len
                     || (*for_s->i + 4 < len
                         && (input[*for_s->i + 4] == ' '
                             || input[*for_s->i + 4] == ';'
                             || input[*for_s->i + 4] == ')'
                             || input[*for_s->i + 4] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_ELIF);
            new[*index - 1].current_tok->value = NULL;
            nb = 3;
        }
        /*
         * testing then
         */
        else if (for_s->mode == 0 && current_char == 't' && *for_s->i + 3 < len
                 && input[*for_s->i + 1] == 'h' && input[*for_s->i + 2] == 'e'
                 && input[*for_s->i + 3] == 'n'
                 && (*for_s->i + 4 == len
                     || (*for_s->i + 4 < len
                         && (input[*for_s->i + 4] == ' '
                             || input[*for_s->i + 4] == ';'
                             || input[*for_s->i + 4] == ')'
                             || input[*for_s->i + 4] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_THEN);
            new[*index - 1].current_tok->value = NULL;
            nb = 3;
        }
        /*
         * testing fi
         */
        else if (for_s->mode == 0 && current_char == 'f' && *for_s->i + 1 < len
                 && input[*for_s->i + 1] == 'i'
                 && (*for_s->i + 2 == len
                     || (*for_s->i + 2 < len
                         && (input[*for_s->i + 2] == ' '
                             || input[*for_s->i + 2] == ';'
                             || input[*for_s->i + 2] == ')'
                             || input[*for_s->i + 2] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FI);
            new[*index - 1].current_tok->value = NULL;
            nb = 1;
        }
        /*
         * testing for
         */
        else if (for_s->mode == 0 && current_char == 'f' && *for_s->i + 2 < len
                 && input[*for_s->i + 1] == 'o' && input[*for_s->i + 2] == 'r'
                 && (*for_s->i + 3 == len
                     || (*for_s->i + 3 < len
                         && (input[*for_s->i + 3] == ' '
                             || input[*for_s->i + 3] == ';'
                             || input[*for_s->i + 3] == ')'
                             || input[*for_s->i + 3] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_FOR);
            new[*index - 1].current_tok->value = NULL;
            nb = 2;
        }
        /*
         * testing in
         */
        else if (for_s->mode == 0 && current_char == 'i' && *for_s->i + 1 < len
                 && input[*for_s->i + 1] == 'n'
                 && (*for_s->i + 2 == len
                     || (*for_s->i + 2 < len
                         && (input[*for_s->i + 2] == ' '
                         || input[*for_s->i + 2] == ')'
                             || input[*for_s->i + 2] == ';'
                             || input[*for_s->i + 2] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_IN);
            new[*index - 1].current_tok->value = NULL;
            *index = *index + 1;
            *for_s->i = *for_s->i + 2;
            get_for(input, for_s, index, new);
            *index = *index - 1;
            *for_s->i = *for_s->i - 1;
            // printf("index here is %zu and i is %zu \n", *index, *for_s->i);
        }
        /*
         * testing Until
         */
        else if (for_s->mode == 0 && current_char == 'u' && *for_s->i + 4 < len
                 && input[*for_s->i + 1] == 'n' && input[*for_s->i + 2] == 't'
                 && input[*for_s->i + 3] == 'i' && input[*for_s->i + 4] == 'l'
                 && (*for_s->i + 5 == len
                     || (*for_s->i + 5 < len
                         && (input[*for_s->i + 5] == ' '
                             || input[*for_s->i + 5] == ';'
                             || input[*for_s->i + 5] == ')'
                             || input[*for_s->i + 5] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_UNTIL);
            new[*index - 1].current_tok->value = NULL;
            nb = 4;
        }
        /*
         * testing Done
         */
        else if (for_s->mode == 0 && current_char == 'd' && *for_s->i + 3 < len
                 && input[*for_s->i + 1] == 'o' && input[*for_s->i + 2] == 'n'
                 && input[*for_s->i + 3] == 'e'
                 && (*for_s->i + 4 == len
                     || (*for_s->i + 4 < len
                         && (input[*for_s->i + 4] == ' '
                             || input[*for_s->i + 4] == ';'
                             || input[*for_s->i + 4] == ')'
                             || input[*for_s->i + 4] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_DONE);
            new[*index - 1].current_tok->value = NULL;
            nb = 3;
        }
        /*
         * testing Do
         */
        else if (for_s->mode == 0 && current_char == 'd' && *for_s->i + 1 < len
                 && input[*for_s->i + 1] == 'o'
                 && (*for_s->i + 2 == len
                     || (*for_s->i + 2 < len
                         && (input[*for_s->i + 2] == ' '
                             || input[*for_s->i + 2] == ';'
                             || input[*for_s->i + 2] == ')'
                             || input[*for_s->i + 2] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_DO);
            new[*index - 1].current_tok->value = NULL;
            nb = 1;
        }
        /*
         * testing While
         */
        else if (for_s->mode == 0 && current_char == 'w' && *for_s->i + 4 < len
                 && input[*for_s->i + 1] == 'h' && input[*for_s->i + 2] == 'i'
                 && input[*for_s->i + 3] == 'l' && input[*for_s->i + 4] == 'e'
                 && (*for_s->i + 5 == len
                     || (*for_s->i + 5 < len
                         && (input[*for_s->i + 5] == ' '
                         || input[*for_s->i + 5] == ')'
                             || input[*for_s->i + 5] == ';'
                             || input[*for_s->i + 5] == '\n'))))
        {
            new[*index - 1].current_tok = token_new(TOKEN_WHILE);
            new[*index - 1].current_tok->value = NULL;
            nb = 4;
        }
        else
        {
            /*
             * testing word
             */
            new[*index - 1].current_tok = token_new(TOKEN_WORDS);
            int var = var2(input, *for_s->i);
            int single_q = 0;
            int double_q = 0;
            nb = 0;
            while (
                *for_s->i + nb < strlen(input)
                && input[*for_s->i + nb] != '<' && input[*for_s->i + nb] != '>'
                && input[*for_s->i + nb] != '|' && input[*for_s->i + nb] != '&'
                && input[*for_s->i + nb] != '}' && input[*for_s->i + nb] != '{'
                && input[*for_s->i + nb] != '(' && input[*for_s->i + nb] != ')')
            {
                //printf("char %c var = %i single q = %i double q = %i \n", input[*for_s->i + nb],
                //var, single_q, double_q);
                    if (var == 1 && input[*for_s->i + nb] == '\'')
                {
                   if (single_q == 0)
                   {
                       single_q = 1;
                   }
                   else
                   {
                        nb++;
                        break;
                   }
                }
                else if (var == 2 && input[*for_s->i + nb] == '\"')
                {
                    if (double_q == 0)
                    {
                        double_q = 1;
                    }
                    else
                    {
                        nb++;
                        break;
                    }
                }
                else if ((var == 0 || var == -1) && is_end(input[*for_s->i + nb]))
                {
                    break;
                }
                
                nb++;
            }
            new[*index - 1].current_tok->value = strndup(input + *for_s->i, nb);
            new[*index - 1].current_tok->value[nb] = '\0';
            if (nb > 0)
            {
                nb--;
            }
        }
    }
    else
    {
        if (!is_end(input[*for_s->i]))
        {
            if (input[*for_s->i] == '&' && *for_s->i + 1 < strlen(input)
                && input[*for_s->i + 1] == '&')
            {
                if (for_s->mode == 1)
                    for_s->mode = 0;
                new[*index - 1].current_tok = token_new(TOKEN_AND);
                new[*index - 1].current_tok->value = NULL;
                nb++;
            }
            else
            {
                // printf("in else %c \n", input[*for_s->i + 0]);
                new[*index - 1].current_tok = token_new(TOKEN_WORDS);
                nb = 0;
                while (*for_s->i + nb < strlen(input)
                       && !is_end(input[*for_s->i + nb])
                       && input[*for_s->i + nb] != '<'
                       && input[*for_s->i + nb] != '>'
                       && input[*for_s->i + nb] != '|'
                       && input[*for_s->i + nb] != '&'
                       && input[*for_s->i + nb] != '}' && input[*for_s->i + nb] != '{'
                && input[*for_s->i + nb] != '(' && input[*for_s->i + nb] != ')')
                {
                    nb++;
                }
                new[*index - 1].current_tok->value =
                    strndup(input + *for_s->i, nb);
                new[*index - 1].current_tok->value[nb] = '\0';
                if (nb > 0)
                {
                    nb--;
                }
            }
        }
    }
    *for_s->i = *for_s->i + nb;
    return for_s;
}
