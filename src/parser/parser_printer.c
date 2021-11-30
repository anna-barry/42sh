#include "parser.h"

#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast)
{
    printf("if root\n");
    struct ast_if_root *a = ast->data.ast_if_root;//(struct ast_if_root *)
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    printf("end if root\n");
}

void print_ast_root(struct ast *ast)
{
    printf("root\n");
    struct ast_main_root *a = ast->data.ast_main_root; //(struct ast_main_root *)
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(a->children[i]);
    }
    printf("end root\n");
}

void print_ast_if(struct ast *ast)
{
    struct ast_if *a = ast->data.ast_if; //(struct ast_if *)
    printf("if { ");
    for (int i = 0; i < a->count_cond; i++)
    {
        print_ast(a->cond[i]);
    }
    printf("}; then {");
    print_ast(a->then);
    printf("; }");
}

void print_ast_elif(struct ast *ast)
{
    struct ast_elif *a = ast->data.ast_elif; //(struct ast_elif *)
    printf("elif { ");
    for (int i = 0; i < a->count_cond; i++)
    {
        print_ast(a->cond[i]);
    }
    printf("}; then {");
    print_ast(a->then);
    printf(" }");
}

void print_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else; //(struct ast_else *)
    printf("else { ");
    print_ast(a->then);
    printf("}");
}

void print_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;//(struct ast_command *)
    printf("command\n");
    for (int i = 0; i < a->count; i++)
    {
        printf(" \"%s\"",a->argv[i]);
    }
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
};

void print_ast(struct ast *ast) {
    printf("ast printing\n");
    ast_printers[ast->type](ast);
}

void pretty_print(struct ast *ast)
{
    printf("here printing\n");
    print_ast(ast);
}

int main()
{
    struct lexer *lexer = lexer_new("if echo ok; then echo foo bar ;elif echo a; then echo bb else echo b fi");
    struct ast *ast = build_ast(lexer);
    printf("\n build ast with nb %i[ok]\n\n", ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    pretty_print(ast);
    return 0;
}
