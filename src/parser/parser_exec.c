

#include "parser_exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "../exec/concat.h"
#include "../functionnal/functionnal.h"
#include "parser.h"

int exec_ast(struct ast *ast, struct environnement *env);

int exec_ast_if(struct ast *ast, struct environnement *env)
{
    // printf("____________________________\nif \n");
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_if *a = ast->data.ast_if;
    int res = exec_ast(a->cond, env);
    // printf("res is: %i\n", res);
    if (env->exit_status != -1)
        return env->exit_status;
    if (res == 0)
    {
        // printf("in then \n");
        exec_ast(a->then, env);
        if (env->exit_status != -1)
            return env->exit_status;
    }
    else
    {
        a->then = NULL;
    }
    // printf("end if \n");
    return res;
}

int exec_ast_if_root(struct ast *ast, struct environnement *env)
{
    //("if root \n");
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("if root\n");
    struct ast_if_root *a = ast->data.ast_if_root;
    for (int i = 0; i < a->nb_children; i++)
    {
        if (env->exit_status != -1)
            return env->exit_status;
        a->status = exec_ast(a->children[i], env);
        // printf("after exec ast %i: status\n", a->status);
        /*if (a->children[i]->type == NODE_IF)
        {
            //printf("\n node if \n");
            if (a->status == 127)
                break;
        }
        if (a->children[i]->type == NODE_ELIF)
        {
            printf("\n node elif \n");
            //if (a->status == 127)
            //break;
        }*/
        if (env->exit_status != -1)
            return env->exit_status;
        if (a->status == 0)
        {
            // printf("in if status %i\n", a->status);
            break;
        }
    }
    // printf("end if root\n");
    return a->status;
}

int exec_ast_and(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("and\n");
    struct ast_and *a = ast->data.ast_and;
    if ((exec_ast(a->left, env) == 0) && (exec_ast(a->right, env) == 0))
    {
        return 0;
    }
    return 1;
}

int exec_ast_or(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("or\n");
    struct ast_or *a = ast->data.ast_or;
    if ((exec_ast(a->left, env) == 0) || (exec_ast(a->right, env) == 0))
    {
        return 0;
    }
    return 1;
}
int exec_ast_neg(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("negation\n");
    struct ast_neg *a = ast->data.ast_neg;
    if (exec_ast(a->node, env) == 1)
        return 0;
    else
        return 1;
}

int exec_ast_while(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct environnement *inter = env;
    struct ast_while *a = ast->data.ast_while;
    int bool_et_bool_et_ratatam = 0;
    env->flag_loop_break += 1;
    int tmp = env->flag_loop_break;
    while (exec_ast(a->cond, inter) == 0)
    {
        if (env->flag_loop_break != tmp)
        {
            bool_et_bool_et_ratatam = 1;
            break;
        }
        if (env->flag_loop_continue != 0)
        {
            env->flag_loop_continue = 0;
            continue;
        }
        else
            exec_ast(a->then, inter);
        if (env->exit_status != -1)
            return env->exit_status;
    }
    if (bool_et_bool_et_ratatam == 0)
        env->flag_loop_break -= 1;
    return 0;
}

int exec_ast_until(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        if (env->exit_status != -1)
            return env->exit_status;
    struct environnement *inter = env;
    struct ast_while *a = ast->data.ast_while;
    while (exec_ast(a->cond, inter) == 0)
    {
        exec_ast(a->then, env);
        if (env->exit_status != -1)
            return env->exit_status;
    }
    return 0;
}

/*
int exec_ast_for(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    struct ast_for *a = ast->data.ast_for;
    struct environnement *e_inter = env;
    if (a->cond->type == NODE_FOR_INT)
    {
        struct ast_for *inter = ast->data.ast_for;
        struct read_for_int *a_inter = a->cond->data.ast_for_int;
        for (int i = a_inter->start; i < a_inter->end; a += a_inter->step)
        {
            char *val = malloc(sizeof(char) * 1000);
            memset(val, '\0', sizeof(char) * 1000);
            itoa(i, val, 10);
            update_variable(a->var, val, e_inter);
            exec_ast(ast->then, inter);
            ast->then = inter->then;
        }
    }
    else if (a->cond->type == NODE_FOR_CHAR)
    {
        struct ast_for_char *a_inter1 = a->cond.ast_for_char->var;
        struct ast_main_root a_inter = a_inter1->data.ast_command;
        int pere = 0;
        for (int i = 1; i < a->nb_children; i++)
        {
            if (a_inter->children[i]->type == NODE_DOUBLE_QUOTE
                || a_inter->children[i]->type == NODE_SIMPLE_QUOTE
                || a_inter->children[i]->type == NODE_COMMAND)
            {
                concat_node(a_inter->children[pere], a_inter->children[i], env);
            }
        }
        struct ast *a_par = ast->then;
        char **command_inter = a_par->argv;
        for (int e = 0; e < a_par->count; e++)
        {
            update_variable(a->var, a_par->argv[e], inter);
            exect_ast(a->then, inter);
            ast->then = a_par;
        }
    }
    return 0;
}
*/

int exec_ast_root(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return 0;
    struct ast_main_root *a = ast->data.ast_main_root;
    int res = -1;
    int inter = 0;
    // printf("nb child = %d\n", a->nb_children);
    for (int i = 0; i < a->nb_children; i++)
    {
        inter = i;
        if (a->children[i]->type == NODE_DOUBLE_QUOTE
            || a->children[i]->type == NODE_SIMPLE_QUOTE
            || a->children[i]->type == NODE_COMMAND)
        {
            // printf("HEEERREEE concat command\n");
            concat_command(a, &i, env);
        }
        // printf("hihihi%d\n", inter);
        /*for (int m = 0; m < as->count; m++)
            printf("ooooooooooooooooooooo %s\n", as->argv[m]);*/
        res = exec_ast(a->children[inter], env);
        if (env->exit_status != -1)
            return env->exit_status;
    }
    return res;
}

int exec_ast_elif(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("elif\n");
    struct ast_elif *a = ast->data.ast_elif;
    int res = exec_ast(a->cond, env);
    if (env->exit_status != -1)
        return env->exit_status;
    if (res == 0)
    {
        exec_ast(a->then, env);
        if (env->exit_status != -1)
            return env->exit_status;
    }
    return res;
}

int exec_ast_else(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("else\n");
    struct ast_else *a = ast->data.ast_else;
    return exec_ast(a->then, env);
}

int exec_ast_pipe(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return 0;
    return pipe_ast(ast, env);
}
/*
int exec_ast_redir(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_redir *a = ast->data.ast_redir;
    enum opt flag = a->opt;
    int return_value = -1;
    switch (flag)
    {
    case REDIR_SORTIE: // '>'
        return_value = ast_redir_r(a->left, a->redir, env);
        break;
    case REDIR_ENTREE: // '<'
        return_value = ast_redir_l(a->left, a->redir, env);
        break;
    case REDIR_FIN_FICHIER: // '>>'
        return_value = ast_redir_rr(a->left, a->redir, env);
        break;
    case REDIR_PIPE: // '>|'
        return_value = ast_redir_r_pipe(a->left, a->redir, env);
        break;
    case REDIR_RW: // '<>'
        return_value = ast_redir_l(a->left, a->redir, env);
        break;
    case REDIR_DESCRIPEUR: // '>&'
        return_value = ast_redir_r_and(a->left, a->redir);
        break;
    case REDIR_INPUT_DESCRIPEUR: // '<&'
        return_value = ast_redir_r_and(a->left, a->redir);
        break;
    }
    if (env->exit_status != -1)
        return env->exit_status;
    return return_value;
}
*/

int exec_ast_command(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_command *a = ast->data.ast_command;
    /*printf("___________________________________\n");
    for (int i = 0; i < a->count; i++)
    {
        printf("command: %s\n", a->argv[i]);
    }*/

    char **tab = get_all_var(a->argv[0]);
    if (tab != NULL)
    {
        // printf("adding %s %s \n", tab[0], tab[1]);
        insert_variable(tab[0], tab[1], env);
        //free(tab);
        free(tab[1]);
        free(tab[0]);
        free(tab);
        return 0;
    }
    free(tab[1]);
    free(tab[0]);
    free(tab);
    enum opt flag = a->opt;
    int return_value = -1;
    switch (flag)
    {
    case REDIR_SORTIE: // '>'
        return_value = command_redir_r(a->argv, a->count, a->redir, env);
        break;
    case REDIR_ENTREE: // '<'
        return_value = command_redir_l(a->argv, a->count, a->redir, env);
        break;
    case REDIR_FIN_FICHIER: // '>>'
        return_value = command_redir_rr(a->argv, a->count, a->redir, env);
        break;
    case REDIR_PIPE: // '>>'
        return_value = command_redir_r_pipe(a->argv, a->count, a->redir, env);
        break;
    case REDIR_RW: // '<>'
        return_value = command_redir_l(a->argv, a->count, a->redir, env);
        break;
    case REDIR_DESCRIPEUR: // '>&'
        return_value = command_redir_r_and(a->argv, a->redir);
        break;
    case REDIR_INPUT_DESCRIPEUR: // '<&'
        return_value = command_redir_r_and(a->argv, a->redir);
        break;
    default:
        return_value = command_exec(a->argv, a->count, env);
    }
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("return value is %i\n", return_value);
    // printf("___________________________________\n");

    return return_value;
}

typedef int (*ast_exec_function)(struct ast *ast, struct environnement *env);

static ast_exec_function ast_exec[] = {
    [NODE_IF] = exec_ast_if,           [NODE_ELIF] = exec_ast_elif,
    [NODE_ELSE] = exec_ast_else,       [NODE_COMMAND] = exec_ast_command,
    [NODE_IF_ROOT] = exec_ast_if_root, [NODE_ROOT] = exec_ast_root,
    [NODE_WHILE] = exec_ast_while,     [NODE_OR] = exec_ast_or,
    [NODE_AND] = exec_ast_and,         [NODE_NEG] = exec_ast_neg,
    [NODE_PIPE] = exec_ast_pipe,
};

int exec_ast(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("exec\n");
    int res = ast_exec[ast->type](ast, env);
    if (env->exit_status != -1)
        return env->exit_status;
    return res;
}

int execution(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    int res = exec_ast(ast, env);
    // printf("end of execution\n");
    if (env->exit_status != -1)
        return env->exit_status;
    return res;
}
