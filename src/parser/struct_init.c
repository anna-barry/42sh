#include "struct_init.h"

#include <stdlib.h>

struct ast_main_root *create_main_root()
{
    struct ast_main_root *ast = malloc(sizeof(struct ast_main_root));
    ast->nb_children = 0;
    //ast->children = malloc(sizeof(struct ast *));
    return ast;
}

struct ast_if_root *create_if_root()
{
    struct ast_if_root *root = malloc(sizeof(struct ast_if_root));
    root->nb_children = 0;
    root->status = 1;
    //root->children = malloc(sizeof(struct ast *));
    return root;
}

struct ast_command *create_command()
{
    struct ast_command *new_c = malloc(sizeof(struct ast_command) * 30);
    new_c->count = 0;
    return new_c;
}


struct ast_pipe *create_pipe()
{
    struct ast_pipe *new = malloc(sizeof(struct ast_pipe));
    new->left = malloc(sizeof(struct ast));
    //new->right = malloc(sizeof(struct ast));
    return new;
}

struct ast_and *create_and()
{
    struct ast_and *new = malloc(sizeof(struct ast_and));
    new->left = malloc(sizeof(struct ast));
    return new;
}

struct ast_or *create_or()
{
    struct ast_or *new = malloc(sizeof(struct ast_or));
    new->left = malloc(sizeof(struct ast));
    return new;
}
