#include "token.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include "alloc.h"
static inline void *xmalloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL && size != 0)
        abort();
    return res;
}


static inline void *zalloc(size_t size)
{
    void *res = xmalloc(size);
    memset(res, 0, size);
    return res;
}

struct token *token_new(enum token_type type)
{
    struct token *new = zalloc(sizeof(struct token));
    new->type = type;
    return new;
}

void token_free(struct token *token)
{
    if (token->value != NULL)
        free(token->value);
    free(token);
}
