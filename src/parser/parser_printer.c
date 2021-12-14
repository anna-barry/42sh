#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast)
{
    printf("if root\n");
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    printf("end if root\n");
}
void print_ast_root(struct ast *ast)
{
    printf("root\n");
    struct ast_main_root *a = ast->data.ast_main_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    printf("end root\n");
}

void print_ast_if(struct ast *ast)
{
    struct ast_if *a = ast->data.ast_if;
    printf("if { ");


    print_ast(a->cond);

    printf("}; then {");
    print_ast(a->then);
    printf("; }");
}

void print_ast_elif(struct ast *ast)
{
    struct ast_elif *a = ast->data.ast_elif;
    printf("elif { ");
    print_ast(a->cond);
    printf("}; then {");
    print_ast(a->then);
    printf(" }");
}

void print_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else;
    printf("else { ");
    print_ast(a->then);
    printf("}");
}

void print_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;
    printf(" command ");
    for (int i = 0; i < a->count; i++)
    {
        printf(" \"%s\"",a->argv[i]);
    }
}

void print_ast_neg(struct ast *ast)
{
    struct ast_neg *a = ast->data.ast_neg;
    printf("! { ");
    print_ast(a->node);
    printf("}");
}

void print_ast_and(struct ast *ast)
{
    struct ast_and *a = ast->data.ast_and;
    printf("{ ");
    print_ast(a->left);
    printf("} && { ");
    print_ast(a->right);
    printf("}");
}

void print_ast_or(struct ast *ast)
{
    struct ast_or *a = ast->data.ast_or;
    printf("{ ");
    print_ast(a->left);
    printf("} || { ");
    print_ast(a->right);
    printf("}");
}

void print_ast_pipe(struct ast *ast)
{
    struct ast_pipe *a = ast->data.ast_pipe;
    printf("{ ");
    print_ast(a->left);
    printf("} | { ");
    print_ast(a->right);
    printf("}");
}

void print_ast_simple_quote(struct ast *ast)
{
    struct ast_simple_quote *a = ast->data.ast_simple_quote;
    printf(" \'%s\' ", a->argv);
}
void print_ast_double_quote(struct ast *ast)
{
    struct ast_double_quote *a = ast->data.ast_double_quote;
    printf(" \"%s\" ", a->argv);
}

void print_ast_while(struct ast *ast)
{
    struct ast_while *a = ast->data.ast_while;
    printf("while ( ");
    print_ast(a->cond);
    printf("); then {");
    print_ast(a->then);
    printf("; }");
}

void print_ast_for(struct ast *ast)
{
    struct ast_for *a = ast->data.ast_for;
    printf("for %s in [ ", a->var);
    print_ast(a->cond);
    printf(" ] do {");
    print_ast(a->then);
    printf(" }");
}

void print_ast_for_char(struct ast *ast)
{
    struct ast_for_char *a = ast->data.ast_for_char;
    print_ast(a->var);
}

void print_ast_for_int(struct ast *ast)
{
    struct read_for_int *a = ast->data.ast_for_int;
    printf(" %lu..%lu..%lu ", a->start, a->end, a->step);
}

void print_ast_redir(struct ast *ast)
{
    struct ast_redir *a = ast->data.ast_redir;
    printf("[redir of type %d] in %s", a->opt, a->redir);
    print_ast(a->command);
    printf("\n end redir\n");
}

typedef void (*ast_print_function)(struct ast *ast);

static ast_print_function ast_printers[] =
{
    [NODE_IF] = print_ast_if,
    [NODE_ELIF] = print_ast_elif,
    [NODE_ELSE] = print_ast_else,
    [NODE_COMMAND] = print_ast_command,
    [NODE_IF_ROOT] = print_ast_if_root,
    [NODE_ROOT] = print_ast_root,
    [NODE_NEG] = print_ast_neg,
    [NODE_AND] = print_ast_and,
    [NODE_OR] = print_ast_or,
    [NODE_PIPE] = print_ast_pipe,
    [NODE_SIMPLE_QUOTE] = print_ast_simple_quote,
    [NODE_DOUBLE_QUOTE] = print_ast_double_quote,
    [NODE_WHILE] = print_ast_while,
    [NODE_FOR] = print_ast_for,
    [NODE_FOR_CHAR] = print_ast_for_char,
    [NODE_FOR_INT] = print_ast_for_int,
    [NODE_REDIR] = print_ast_redir,
};

void print_ast(struct ast *ast) {
    ast_printers[ast->type](ast);
}

void my_pretty_print(struct ast *ast)
{
    print_ast(ast);
}

