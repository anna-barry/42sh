#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/echo.h"
#include "../functionnal/functionnal.h"
#include "parser.h"
#include "parser_exec.h"

int main()
{
    struct environnement *var = malloc(sizeof(struct environnement));
    insert_variable("a", "abba", var);
    insert_variable("b", "bac", var);
    insert_variable("c", "coucou", var);
    struct lexer *lexer = lexer_new("if echo $a b ; then echo 'foo' bar ; fi");
    // struct lexer *lexer = lexer_new("if echo $a $b ; then echo 'foo' bar ;
    // fi");
    //   struct lexer *lexer = lexer_new("if echo $a ; then echo 'foo' bar ;
    //   fi");
    //    struct lexer *lexer = lexer_new("if echo b; echo a ; then echo abb ;
    //    fi"); struct lexer *lexer = lexer_new("until echo a do echo b done");
    struct ast *ast = build_ast(lexer, NODE_ROOT);
    printf("\n build ast with nb %i[ok]\n\n",
           ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    execution(ast, var);
    lexer_free(lexer);
    return 0;
}
