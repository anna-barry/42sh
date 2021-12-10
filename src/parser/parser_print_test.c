#include "parser_printer.h"
#include "parser.h"
#include "parser_free.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"

#include <stdio.h>

int main()
{
    //struct lexer *lexer = lexer_new("for i in {1..10}; do printf \"a \n\" ; done");
    //struct lexer *lexer = lexer_new("if echo ok && echo a; then echo foo bar ; else echo b ;fi");
    //struct lexer *lexer = lexer_new("for i inword 'ok' \"test\"; do printf \"a \n\" ; done");
    //struct lexer *lexer = lexer_new("if echo a; if echo b ; then echo b ; fi; then echo merci; else echo c; fi");
    //struct lexer *lexer = lexer_new("Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;");
    //struct lexer *lexer = lexer_new("if false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; else echo b; fi");
    //struct lexer *lexer = lexer_new("if echo \"ls\"; then echo 'foo' bar; echo okay; elif echo bebe; echo a ; then echo christian; echo anna laime; fi");
    //struct lexer *lexer = lexer_new("if echo b; echo a; then echo a; fi; else echo a");
    //struct lexer *lexer = lexer_new("while echo a || echo b; do echo b;");
    //struct lexer *lexer = lexer_new("until echo a || echo b; do echo b; done");
    //struct lexer *lexer = lexer_new("until echo a || echo b; do echo b; done");
    //struct lexer *lexer = lexer_new("for i in word; do echo a; done");
    struct lexer *lexer = lexer_new("if ! echo a; then echo b; fi");
    struct ast *ast = build_ast(lexer, NODE_ROOT);
    /*printf("\n build ast with nb %i[ok]\n\n", ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");*/
    my_pretty_print(ast);
    my_pretty_free(ast);
    lexer_free(lexer);
    return 0;
}
