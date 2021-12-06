#include "parser_exec.h"
#include "../commands/command.h"
#include "parser.h"

int main()
{
    struct lexer *lexer =
        lexer_new("if echo a then ls elif echo bebe then echo anna; fi");
    struct ast *ast = build_ast(lexer, NODE_ROOT);
    printf("\n build ast with nb %i[ok]\n\n",
           ast->data.ast_main_root->nb_children);
    if (ast->type == NODE_ROOT)
    {
        printf("node root\n");
    }
    printf("before pretty print\n");
    int res_e = execution(ast);
    lexer_free(lexer);
    return 0;
}