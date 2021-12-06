//#include "parser_exec.h"

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "parser.h"

int exec_ast(struct ast *ast);

int exec_ast_if_root(struct ast *ast)
{
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        a->status = exec_ast(a->children[i]);
        if (a->status == 0)
            break;
    }
    return a->status;
}

int exec_ast_root(struct ast *ast)
{
    struct ast_main_root *a = ast->data.ast_main_root;
    int res = -1;
    for (int i = 0; i < a->nb_children; i++)
    {
        res = exec_ast(a->children[i]);
    }
    return res;
}

int exec_ast_and(struct ast *ast)
{
    struct ast_and *a = ast->data.ast_and;
    if ((exec_ast(a->right) == 0) && (exec_ast(a->left) == 0))
    {
        return 0;
    }
    return 1;
}

int exec_ast_or(struct ast *ast)
{
    struct ast_or *a = ast->data.ast_or;
    if ((exec_ast(a->right) == 0) || (exec_ast(a->left) == 0))
    {
        return 0;
    }
    return 1;
}

int exec_ast_if(struct ast *ast)
{
    struct ast_if *a = ast->data.ast_if;
    int res = exec_ast(a->cond);
    int inter = 0;
    if (res == 0)
    {
        inter = exec_ast(a->then);
    }
    return res;
}

int exec_ast_elif(struct ast *ast)
{
    struct ast_elif *a = ast->data.ast_elif;
    int res = exec_ast(a->cond);
    int inter = 0;
    if (res == 0)
    {
        inter = exec_ast(a->then);
    }
    return res;
}

int exec_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else;
    return exec_ast(a->then);
}

int exec_ast_pipe(struct ast *ast)
{
    struct ast_pipe *a = ast->data.ast_pipe;
    struct ast_command *left = a->left;
    struct ast_command *right = a->right;
    return pipe_exec(left->argv, left->count, right->argv, right->count);
}

int exec_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;
    enum option flag = a->option;
    int return_value = -1;
    switch (flag)
    {
    case REDIR_SORTIE: // '>'
        return_value = command_redir_r(a->argv, a->count, a->redir);
        break;
    case REDIR_ENTREE: // '<'
        return_value = command_redir_l(a->argv, a->count, a->redir);
        break;
    case REDIR_FIN_FICHIER: // '>>'
        return_value = command_redir_rr(a->argv, a->count, a->redir);
        break;
    case REDIR_RW: // '<>'
        return_value = command_redir_l(a->argv, a->count, a->redir);
        break;
    case REDIR_DESCRIPEUR: // '>&'
        return_value = command_redir_r_and(a->argv[0], a->redir);
        break;
    case REDIR_INPUT_DESCRIPEUR: // '<&'
        return_value = command_redir_r_and(a->argv[0], a->redir);
        break;
    default:
        return_value = command_exec(a->argv, a->count);
    }
    return return_value;
}

typedef int (*ast_exec_function)(struct ast *ast);

static ast_exec_function ast_exec[] = {
    [NODE_IF] = exec_ast_if,           [NODE_ELIF] = exec_ast_elif,
    [NODE_ELSE] = exec_ast_else,       [NODE_COMMAND] = exec_ast_command,
    [NODE_IF_ROOT] = exec_ast_if_root, [NODE_ROOT] = exec_ast_root,
};

int exec_ast(struct ast *ast)
{
    return ast_exec[ast->type](ast);
}

int execution(struct ast *ast)
{
    return exec_ast(ast);
}
/*
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
*/
