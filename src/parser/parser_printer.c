#include "parser.h"

#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast)
{
    printf("if root\n");
    struct ast_if_root *a = (struct ast_if_root *) ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(&a->children[i].type);
    }
    printf("end if root\n");
}

void print_ast_root(struct ast *ast)
{
    printf("root\n");
    struct ast_main_root *a = (struct ast_main_root *) ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        print_ast(&a->children[i].type);
    }
    printf("end root\n");
}

void print_ast_if(struct ast *ast)
{
    struct ast_if *a = (struct ast_if *) ast;
    printf("if { ");
    for (int i = 0; i < a->count_cond; i++)
    {
        print_ast(&a->cond[i].type);
    }
    printf("}; then {");
    print_ast(&a->then->type);
    printf("; }");
}

void print_ast_elif(struct ast *ast)
{
    struct ast_elif *a = (struct ast_elif *) ast;
    printf("elif { ");
    for (int i = 0; i < a->count_cond; i++)
    {
        print_ast(&a->cond[i].type);
    }
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
    printf("ast printing");
    ast_printers[ast->type](ast);
}

void pretty_print(struct ast_main_root *ast)
{
    printf("here printing");
    print_ast(&ast->type);
}

int main()
{
    struct lexer *lexer = lexer_new("if echo ok; then echo foobar; fi");
    struct ast_main_root *ast = build_ast(lexer);
    printf("\n build ast with nb %i[ok]\n\n", ast->nb_children);
    if (ast->type.type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    pretty_print(ast);
    return 0;
}
