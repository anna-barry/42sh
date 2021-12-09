#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "parser_free.h"

void free_ast(struct ast *ast);

void free_ast_if_root(struct ast *ast)
{
    //freef("if root\n");
    /*
    *struct ast_if_root
{
    int nb_children;
    int status;
    struct ast **children;
};
    */
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        free_ast(a->children[i]);
    }
    printf("here \n");
    if (a->children)
        free(a->children);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("end if root\n");
}

void free_ast_root(struct ast *ast)
{
    //freef("root\n");
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
    //freef("end root\n");
}

void free_ast_if(struct ast *ast)
{
    struct ast_if *a = ast->data.ast_if;
    //freef("if { ");
    free_ast(a->cond);

    //freef("}; then {");
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("; }");
}

void free_ast_elif(struct ast *ast)
{
    struct ast_elif *a = ast->data.ast_elif;
    //freef("elif { ");
    free_ast(a->cond);
    //freef("}; then {");
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef(" }");
}

void free_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else;
    //freef("else { ");
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("}");
}

void free_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;
    for (int i = 0; i < a->count; i++)
        free(a->argv[i]);
    free(a->argv);
    if (a->opt != NONE)
      free(a->redir);
    if (a)
        free(a);
    if (ast)
        free(ast);
}

void free_ast_neg(struct ast *ast)
{
    struct ast_neg *a = ast->data.ast_neg;
    //freef("! { ");
    free_ast(a->node);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("}");
}

void free_ast_and(struct ast *ast)
{
    struct ast_and *a = ast->data.ast_and;
    //freef("{ ");
    free_ast(a->left);
    //freef("} && { ");
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("}");
}

void free_ast_or(struct ast *ast)
{
    struct ast_or *a = ast->data.ast_or;
    //freef("{ ");
    free_ast(a->left);
    //freef("} || { ");
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("}");
}

void free_ast_pipe(struct ast *ast)
{
    struct ast_pipe *a = ast->data.ast_pipe;
    //freef("{ ");
    free_ast(a->left);
    //freef("} | { ");
    free_ast(a->right);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("}");
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
    //freef("while ( ");
    free_ast(a->cond);
    //freef("); then {");
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef("; }");
}

void free_ast_for(struct ast *ast)
{
    struct ast_for *a = ast->data.ast_for;
    //freef("for %s in [ ", a->var);
    free(a->var);
    free_ast(a->cond);
    //freef(" ] do {");
    free_ast(a->then);
    if (a)
        free(a);
    if (ast)
        free(ast);
    //freef(" }");
}

void free_ast_for_char(struct ast *ast)
{
    struct ast_for_char *a = ast->data.ast_for_char;
    free_ast(a->var);
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
    [NODE_FOR_CHAR] = free_ast_for_char,
    [NODE_FOR_INT] = free_ast_for_int,
};

void free_ast(struct ast *ast) {
    ast_freeers[ast->type](ast);
    //free(ast);
}

void my_pretty_free(struct ast *ast)
{
    free_ast(ast);
   // if (ast)
    //    free(ast);
}
