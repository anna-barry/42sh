#include "lexer.h"
#include <stdlib.h>

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
    if(info->cap)
        free(info->cap);
    if(info->index)
        free(info->index);
    if (info->lexer)
        lexer_free(info->lexer);
    if (info)
        free(info);
}

void clear_info(struct info_lexer *new)
{
    struct token *token = lexer_pop(new->lexer);
    while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
    {
        token_free(token);
        token = lexer_pop(new->lexer);
    }
    token_free(token);
    *new->index = 1;
}
