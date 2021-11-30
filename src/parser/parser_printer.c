#include "parser.h"

#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast)
{
    struct ast_if_root *a = (struct ast_if_root *) ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(&a->children->ast_if_root->type);
    }
}

void print_ast_root(struct ast *ast)
{
    struct ast_main_root *a = (struct ast_main_root *) ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(&a->children->ast_if_root->type);
    }
}

void print_ast_if(struct ast *ast)
{
    struct ast_if *a = (struct ast_if *) ast;
    printf("if { ");
    print_ast(&a->cond->type);
    printf("}; then {");
    print_ast(&a->then->type);
    printf("; }");
}

void print_ast_elif(struct ast *ast)
{
    struct ast_elif *a = (struct ast_elif *) ast;
    printf("elif { ");
    print_ast(&a->cond->type);
    printf("}; then {");
    print_ast(&a->then->type);
    printf(" }");
}

void print_ast_else(struct ast *ast)
{
    struct ast_else *a = (struct ast_else *) ast;
    printf("else { ");
    print_ast(&a->then->type);
    printf("}");
}

void print_ast_command(struct ast *ast)
{
    struct ast_command *a = (struct ast_command *) ast;
    printf("command");
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
    ast_printers[ast->type](ast);
}

void pretty_print(struct ast_main_root *ast)
{
    print_ast(&ast->type);
}

int main()
{
    struct lexer *lexer = lexer_new("if echo b; echo a; then echo a; fi");
    struct ast_main_root *ast = build_ast(lexer);
    printf("OKAY\n");
    pretty_print(ast);
    return 0;
}
