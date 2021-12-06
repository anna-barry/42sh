#include "parser_printer.h"
#include "parser.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"

#include <stdio.h>

int main()
{
    //struct lexer *lexer = lexer_new("if echo ok; then echo foo bar else echo b ;fi");
    struct lexer *lexer = lexer_new("if echo ok ;echo celib then echo 'foo bar'; echo okay; elif echo bebe then echo christian; echo anna laime; fi");
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