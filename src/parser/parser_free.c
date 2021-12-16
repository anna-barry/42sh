#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "parser_free.h"

void free_ast(struct ast *ast);

void free_ast_if_root(struct ast *ast)
{
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        free_ast(a->children[i]);
    }
    if (a->children)
        free(a->children);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_root(struct ast *ast)
{
    struct ast_main_root *a = ast->data.ast_main_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        free_ast(a->children[i]);
    }
    if (a->children)
        free(a->children);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_if(struct ast *ast)
{
    struct ast_if *a = ast->data.ast_if;
    free_ast(a->cond);
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_elif(struct ast *ast)
{
    struct ast_elif *a = ast->data.ast_elif;
    free_ast(a->cond);
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else;
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;
    for (int i = 0; i < a->count; i++)
        free(a->argv[i]);
    free(a->argv);
    free(a->type);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_neg(struct ast *ast)
{
    struct ast_neg *a = ast->data.ast_neg;
    free_ast(a->node);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_and(struct ast *ast)
{
    struct ast_and *a = ast->data.ast_and;
    free_ast(a->left);
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_or(struct ast *ast)
{
    struct ast_or *a = ast->data.ast_or;
    free_ast(a->left);
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_pipe(struct ast *ast)
{
    struct ast_pipe *a = ast->data.ast_pipe;
    free_ast(a->left);
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_simple_quote(struct ast *ast)
{
    struct ast_simple_quote *a = ast->data.ast_simple_quote;
    free(a->argv);
    if (a)
        free(a);
    if (ast)
        free(ast);
}
void free_ast_double_quote(struct ast *ast)
{
    struct ast_double_quote *a = ast->data.ast_double_quote;
    free(a->argv);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_while(struct ast *ast)
{
    struct ast_while *a = ast->data.ast_while;
    free_ast(a->cond);
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_for(struct ast *ast)
{
    struct ast_for *a = ast->data.ast_for;
    free(a->var);
    free_ast(a->cond);
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_for_int(struct ast *ast)
{
    struct read_for_int *a = ast->data.ast_for_int;
    free(a);
    if (ast)
        free(ast);
}

void free_ast_redir(struct ast *ast)
{
    struct ast_redir *a = ast->data.ast_redir;
    free(a);
    free(a->command);
    free(a->redir);
    if (ast)
        free(ast);
}

static ast_free_function ast_freeers[] =
{
    [NODE_IF] = free_ast_if,
    [NODE_ELIF] = free_ast_elif,
    [NODE_ELSE] = free_ast_else,
    [NODE_COMMAND] = free_ast_command,
    [NODE_IF_ROOT] = free_ast_if_root,
    [NODE_ROOT] = free_ast_root,
    [NODE_NEG] = free_ast_neg,
    [NODE_AND] = free_ast_and,
    [NODE_OR] = free_ast_or,
    [NODE_PIPE] = free_ast_pipe,
    [NODE_SIMPLE_QUOTE] = free_ast_simple_quote,
    [NODE_DOUBLE_QUOTE] = free_ast_double_quote,
    [NODE_WHILE] = free_ast_while,
    [NODE_FOR] = free_ast_for,
    [NODE_FOR_INT] = free_ast_for_int,
    [NODE_REDIR] = free_ast_redir,
};

void free_ast(struct ast *ast) {
    ast_freeers[ast->type](ast);
}

void my_pretty_free(struct ast *ast)
{
    free_ast(ast);
}
