#include "parser_printer.h"
#include "parser.h"
#include "parser_free.h"
#include "../lexer/token.h"
//#include "../lexer/lexer_added.c"
#include "../lexer/lexer.h"

#include <stdio.h>

int main()
{
    //char *input = "for i in {1..10}; do printf \"a \n\" ; done";
    //char *input = "if echo ok && echo a; then echo foo bar ; else echo b ;fi";
    //char *input = "for i inword 'ok' \"test\"; do printf \"a \n\" ; done";
    //char *input = "if echo a; if echo b ; then echo b ; fi; then echo merci; else echo c; fi";
    //char *input = "Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e; Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;Echo a; echo b; echo c; echo d; echo e;";
    //char *input = "if false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; elif false; then echo a; else echo b; fi";
    //char *input = "if echo \"ls\"; then echo 'foo' bar; echo okay; elif echo bebe; echo a ; then echo christian; echo anna laime; fi";
    //char *input = "if echo b; echo a; then echo a; fi; else echo a";
    char *input = "while echo a || echo b; do echo b;";
    //char *input = "until echo a || echo b; do echo b; done";
    //char *input = "until echo a || echo b; do echo b; done";
    //char *input = "for i in word; do echo a; done";
    //char *input = "if ! echo a; then echo b; fi";
    //char *input = "if echo c; then echo b; else echo a; fi; | cat -e | sort";
    //char *input = "if echo c; then echo b; else echo a; fi; | cat -e";
    //char *input = "ls -al > listings";
    //char *input = "cat music.mp3 > /dev/audio";
    //char *input = "Mail -s \"Subject\" to-address < Filenam";
    //char *input = "myprogram 2 >errorsfile";
    //char *input = "ls pt ABC > dirlist 2 >&1";
    //char *input = "if [ 1 -eq 2 ];then echo aa; elif echo baba; then echo b; fi";
    //char *input = "for i in word; do echo a; done > cat";
    struct info_lexer *new = lexer_init();
    lexer_new(input, new);
    struct ast *ast = build_ast(new, NODE_ROOT);
    /*printf("\n build ast with nb %i[ok]\n\n", ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");*/
    my_pretty_print(ast);
    my_pretty_free(ast);
    token_free(lexer_pop(new->lexer));
    lexer_info_free(new);
    return 0;
}
