#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast)
{
    //printf("if root\n");
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    //printf("end if root\n");
}

void print_ast_root(struct ast *ast)
{
    //printf("root\n");
    struct ast_main_root *a = ast->data.ast_main_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    //printf("end root\n");
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
    printf("command\n");
    for (int i = 0; i < a->count; i++)
    {
        printf(" \"%s\"",a->argv[i]);
        switch (a->option)
        {
            case (REDIR_SORTIE):
            printf(">%s", a->redir);
            break;
            case (REDIR_ENTREE):
            printf("<%s", a->redir);
            break;
            case (REDIR_DESCRIPEUR):
            printf(">&%s", a->redir);
            break;
            case (REDIR_FIN_FICHIER):
            printf(">>%s", a->redir);
            break;
            case (REDIR_INPUT_DESCRIPEUR):
            printf("<&%s", a->redir);
            break;
            case (REDIR_RW):
            printf("<>%s", a->redir);
            break;
            case (TOKEN_REDIR_PIPE):
            printf(">|%s", a->redir);
            break;
            default:
            break;
        }
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
    printf("} && { ");
    print_ast(a->right);
    printf("}");
}

void print_ast_simple_quote(struct ast *ast)
{
    struct ast_simple_quote *a = ast->data.ast_simple_quote;
    printf("%s", a->argv);
}
void print_ast_double_quote(struct ast *ast)
{
    struct ast_double_quote *a = ast->data.ast_double_quote;
    printf("%s", a->argv);
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
};

void print_ast(struct ast *ast) {
    printf("ast printing\n");
    ast_printers[ast->type](ast);
    free(ast);
}

void pretty_print(struct ast *ast)
{
    printf("here printing\n");
    print_ast(ast);
}

int main()
{
    //struct lexer *lexer = lexer_new("if echo ok; then echo foo bar else echo b ;fi");
    //struct lexer *lexer = lexer_new("if echo ok; then echo foo bar; echo okay; elif echo bebe; echo a ;then echo christian; echo anna laime; fi");
    //struct lexer *lexer = lexer_new("if echo b; echo a then echo a fi else echo a");
    //struct lexer *lexer = lexer_new("until echo a do echo b done");
    struct lexer *lexer = lexer_new("for i in word do echo a done");
    struct ast *ast = build_ast(lexer, NODE_ROOT);
    printf("\n build ast with nb %i[ok]\n\n", ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    pretty_print(ast);
    lexer_free(lexer);
    return 0;
}
