

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
    /*else
    {
        a->then = NULL;
    }*/
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
    struct environnement *inter = copy_env(env);
    struct ast_while *a = ast->data.ast_while;
    while (exec_ast(a->cond, inter) == 0)
    {
        if (inter->exit_status != -1)
            return inter->exit_status;
        if (inter->flag_loop_break != 0)
        {
            inter->flag_loop_break = 0;
            break;
        }
        else if (inter->flag_loop_continue != 0)
        {
            inter->flag_loop_continue = 0;
            continue;
        }
        else
            exec_ast(a->then, inter);

        if (inter->exit_status != -1)
            return inter->exit_status;
        if (inter->flag_loop_break != 0)
        {
            inter->flag_loop_break = 0;
            break;
        }
        if (inter->flag_loop_continue != 0)
        {
            inter->flag_loop_continue = 0;
            continue;
        }
    }
    env->flag_loop_break = inter->flag_loop_break;
    env->flag_loop_continue = inter->flag_loop_continue;
    env->exit_status = inter->exit_status;
    free_environnement(inter);
    return 0;
}

/*

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
*/

int exec_ast_for(struct ast *ast, struct environnement *env)
{
    // printf("for node\n");
    if (env == NULL)
        return 1;
    struct ast_for *a = ast->data.ast_for;
    if (a->cond == NULL)
        return 0;
    struct environnement *e_inter = copy_env(env);
    if (a->cond->type == NODE_FOR_INT)
    {
        // printf("for int node\n");
        char *var_inter = NULL;
        var_inter = strndup(a->var, strlen(a->var));
        struct ast_for *inter = ast->data.ast_for;
        struct read_for_int *a_inter = a->cond->data.ast_for_int;
        for (int i = a_inter->start; i <= (int)a_inter->end; i += a_inter->step)
        {
            if (e_inter->exit_status != -1)
                return e_inter->exit_status;
            char *val = malloc(sizeof(char) * 1000);
            memset(val, '\0', sizeof(char) * 1000);
            val = itoa(i, val);
            update_variable(var_inter, val, e_inter);
            if (e_inter->flag_loop_break != 0)
            {
                e_inter->flag_loop_break = 0;
                break;
            }
            else if (e_inter->flag_loop_continue != 0)
            {
                e_inter->flag_loop_continue = 0;
                continue;
            }
            else
                exec_ast(a->then, e_inter);

            a->then = inter->then;

            if (e_inter->exit_status != -1)
                return e_inter->exit_status;
            if (e_inter->flag_loop_break != 0)
            {
                e_inter->flag_loop_break = 0;
                break;
            }
            if (e_inter->flag_loop_continue != 0)
            {
                e_inter->flag_loop_continue = 0;
                continue;
            }
        }
    }
    else if (a->cond->type == NODE_ROOT)
    {
        struct ast_main_root *a_inter = a->cond->data.ast_main_root;
        int pere = 0;
        for (int i = 1; i < a_inter->nb_children; i++)
        {
            if (a_inter->children[i]->type == NODE_DOUBLE_QUOTE
                || a_inter->children[i]->type == NODE_SIMPLE_QUOTE
                || a_inter->children[i]->type == NODE_COMMAND)
            {
                concat_node(a_inter->children[pere], a_inter->children[i],
                            e_inter);
            }
        }
        // struct ast *a_par = malloc(sizeof(*ast));
        char *var_inter = NULL;
        var_inter = strndup(a->var, strlen(a->var));
        //  my_pretty_print(a_par);
        struct ast_command *a_interme = a_inter->children[0]->data.ast_command;
        for (int e = 0; e < a_interme->count; e++)
        {
            // printf("voici elt = %s\n", a_interme->argv[e]);
            if (a_interme->argv[e] != NULL)
            {
                if (e_inter->exit_status != -1)
                    return e_inter->exit_status;
                char *elt_inter = NULL;
                elt_inter =
                    strndup(a_interme->argv[e], strlen(a_interme->argv[e]));
                update_variable(var_inter, elt_inter, e_inter);
                if (e_inter->flag_loop_break != 0)
                {
                    e_inter->flag_loop_break = 0;
                    if (e < a_interme->count - 1)
                        free(elt_inter);
                    break;
                }
                else if (e_inter->flag_loop_continue != 0)
                {
                    e_inter->flag_loop_continue = 0;
                    if (e < a_interme->count - 1)
                        free(elt_inter);
                    continue;
                }
                else
                    exec_ast(a->then, e_inter);

                if (e_inter->exit_status != -1)
                {
                    if (e < a_interme->count - 1)
                        free(elt_inter);
                    return e_inter->exit_status;
                }
                if (e_inter->flag_loop_break != 0)
                {
                    e_inter->flag_loop_break = 0;
                    if (e < a_interme->count - 1)
                        free(elt_inter);
                    break;
                }
                if (e_inter->flag_loop_continue != 0)
                {
                    e_inter->flag_loop_continue = 0;
                    if (e < a_interme->count - 1)
                        free(elt_inter);
                    continue;
                }
                if (e < a_interme->count - 1)
                    free(elt_inter);
            }
        }
        // free(a_par);
    }
    env->flag_loop_break = e_inter->flag_loop_break;
    env->flag_loop_continue = e_inter->flag_loop_continue;
    env->exit_status = e_inter->exit_status;
    free_environnement(e_inter);
    return 0;
}

int exec_ast_root(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return 0;
    struct ast_main_root *a = ast->data.ast_main_root;
    int res = -1;
    // int inter = 0;
    //  printf("nb child = %d\n", a->nb_children);
    for (int i = 0; i < a->nb_children; i++)
    { /*
         inter = i;
         if (a->children[i]->type == NODE_DOUBLE_QUOTE
             || a->children[i]->type == NODE_SIMPLE_QUOTE
             || a->children[i]->type == NODE_COMMAND)
         {
             // printf("HEEERREEE concat command\n");
             concat_command(a, &i, env);
         }*/
        // printf("hihihi%d\n", inter);
        /*for (int m = 0; m < as->count; m++)
            printf("ooooooooooooooooooooo %s\n", as->argv[m]);*/
        res = exec_ast(a->children[i], env);
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
        return_value = ast_redir_r(ast, a->redir, env);
        break;
    case REDIR_ENTREE: // '<'
        return_value = ast_redir_l(ast, a->redir, env);
        break;
    case REDIR_FIN_FICHIER: // '>>'
        return_value = ast_redir_rr(ast, a->redir, env);
        break;
    case REDIR_PIPE: // '>|'
        return_value = ast_redir_r_pipe(ast, a->redir, env);
        break;
    case REDIR_RW: // '<>'
        return_value = ast_redir_l(ast, a->redir, env);
        break;
    case REDIR_DESCRIPEUR: // '>&'
        return_value =
            command_redir_r_and(a->command->data.ast_command->argv, a->redir);
        break;
    case REDIR_INPUT_DESCRIPEUR: // '<&'
        return_value =
            command_redir_r_and(a->command->data.ast_command->argv, a->redir);
        break;
    }
    if (env->exit_status != -1)
        return env->exit_status;
    return return_value;
}

int exec_ast_command(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_command *a = ast->data.ast_command;
    // printf("___________________________________\n");

    // int_variables(env);
    char **tab = get_all_var(a->argv[0]);
    if (tab != NULL)
    {
        // printf("adding %s %s \n", tab[0], tab[1]);
        // char *z = strndup(tab[0], strlen(tab[0]));
        // char *f = strndup(tab[1], strlen(tab[1]));
        // printf("tab%s");
        insert_variable(tab[0], tab[1], env);
        // print_variables(env);
        // free(tab);
        /*if (tab[1] != NULL)
            free(tab[1]);
        if (tab[0] != NULL)
            free(tab[0]);*/
        free(tab);
        return 0;
    }
    // free(tab[1]);
    // free(tab[0]);
    // free(tab);
    // enum opt flag = a->opt;
    int return_value = -1;
    // switch (flag)
    // {
    // case REDIR_SORTIE: // '>'
    //     return_value = command_redir_r(a->argv, a->count, a->redir, env);
    //     break;
    // case REDIR_ENTREE: // '<'
    //     return_value = command_redir_l(a->argv, a->count, a->redir, env);
    //     break;
    // case REDIR_FIN_FICHIER: // '>>'
    //     return_value = command_redir_rr(a->argv, a->count, a->redir,
    //     env); break;
    // case REDIR_PIPE: // '>>'
    //     return_value = command_redir_r_pipe(a->argv, a->count, a->redir,
    //     env); break;
    // case REDIR_RW: // '<>'
    //     return_value = command_redir_l(a->argv, a->count, a->redir, env);
    //     break;
    // case REDIR_DESCRIPEUR: // '>&'
    //     return_value = command_redir_r_and(a->argv, a->redir);
    //     break;
    // case REDIR_INPUT_DESCRIPEUR: // '<&'
    //     return_value = command_redir_r_and(a->argv, a->redir);
    //     break;
    // default:
    if (env->flag_loop_break == 0 && env->flag_loop_continue == 0)
        return_value = command_exec(ast, a->count, env);
    else
        return_value = 2;
    // }
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
    [NODE_PIPE] = exec_ast_pipe,       [NODE_REDIR] = exec_ast_redir,
    [NODE_FOR] = exec_ast_for,
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
