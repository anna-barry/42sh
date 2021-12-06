#include "parser_printer.h"
#include "parser.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"

#include <stdio.h>

int main()
{
    //struct lexer *lexer = lexer_new("if echo ok && echo a; then echo foo bar ; else echo b ;fi");
    struct lexer *lexer = lexer_new("if echo ok ; then echo foo bar ; echo okay ; elif echo bebe ; echo a ;then echo christian ; echo anna laime ; fi");
    //struct lexer *lexer = lexer_new("if echo b; echo a then echo a fi else echo a");
    //struct lexer *lexer = lexer_new("while echo a || echo b; do echo b; done");
    //struct lexer *lexer = lexer_new("for i; in word; do echo a; done");
    struct ast *ast = build_ast(lexer, NODE_ROOT);
    printf("\n build ast with nb %i[ok]\n\n", ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    my_pretty_print(ast);
    lexer_free(lexer);
    return 0;
}