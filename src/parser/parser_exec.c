

#include "parser_exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "../functionnal/functionnal.h"
#include "parser.h"

int exec_ast(struct ast *ast, struct environnement *env);

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
        if (env->exit_status != -1)
            return env->exit_status;
        if (a->status == 0)
            break;
    }
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

int is_nullf(struct ast *ast)
{
    if (ast->type == NODE_COMMAND)
    {
        struct ast_command *a = ast->data.ast_command;
        for (int index = 0; index < a->count; index++)
        {
            if (a->argv[index] == NULL)
            {
                return 1;
            }
        }
        return 0;
    }
    else if (ast->type == NODE_DOUBLE_QUOTE)
    {
        struct ast_double_quote *a = ast->data.ast_double_quote;
        if (a->argv == NULL)
        {
            return 1;
        }
        return 0;
    }
    else if (ast->type == NODE_SIMPLE_QUOTE)
    {
        struct ast_simple_quote *a = ast->data.ast_simple_quote;
        if (a->argv == NULL)
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

char *transform_char(char *argv, struct environnement *env, int *index)
{
    size_t cap = 200;
    char *indice = malloc(sizeof(char) * cap);
    memset(indice, '\0', sizeof(char) * cap);
    int start = *index;
    unsigned long interm = *index + 1;
    size_t p = 0;
    // printf("interm %lu *argv[0]=%s \n", interm, argv);
    // printf("%lu stren is \n", strlen(argv));
    for (; interm < strlen(argv) || argv[interm] != ' '; interm++)
    {
        indice[p] = argv[interm];
        p += 1;
        if (p >= cap)
        {
            cap *= 2;
            indice = realloc(indice, cap * sizeof(char));
        }
        if (interm == strlen(argv))
        {
            break;
        }
    }
    indice[interm] = '\0';
    struct variable *inter = env->var;
    while (inter)
    {
        // printf("inter is %s\n", inter->name);
        if (strcmp(indice, inter->name) == 0)
            break;
        inter = inter->next;
    }
    free(indice);
    if (inter == NULL)
    {
        char *res = malloc(sizeof(char));
        res = NULL;
        free(argv);
        return res;
    }
    cap = 200;
    char *res = malloc(sizeof(char) * cap);
    memset(res, '\0', sizeof(char) * cap);
    size_t a = 0;
    for (; (int)a < start; a++)
    {
        res[a] = argv[a];
        if (a >= cap)
        {
            cap *= 2;
            res = realloc(res, sizeof(char) * cap);
        }
    }
    size_t avance = a;
    for (unsigned long f = 0; f < strlen(inter->value); f++)
    {
        res[avance] = inter->value[f];
        avance += 1;
        if (avance >= cap)
        {
            cap *= 2;
            res = realloc(res, sizeof(char) * cap);
        }
    }
    avance += 1;
    a += strlen(inter->name) + 1;
    for (; argv[a] != '\0'; a++)
    {
        res[avance] = argv[a];
        avance += 1;
        if (avance >= cap)
        {
            cap *= 2;
            res = realloc(res, sizeof(char) * cap);
        }
    }
    res[avance] = '\0';
    *index += (strlen(inter->value) - strlen(inter->name));
    free(argv);
    return res;
}

void transform_command(struct ast *ast, struct environnement *env)
{
    // printf("transform command\n");
    if (env == NULL)
        return;
    if (ast->type == NODE_DOUBLE_QUOTE)
    {
        struct ast_double_quote *a = ast->data.ast_double_quote;
        int index = 0;
        for (; a->argv[index] != '\0'; index++)
        {
            if (a->argv[index] == '$')
            {
                char *new = transform_char(a->argv, env, &index);
                // printf("new char is %s\n", new);
                a->argv = new;
            }
        }
    }
    else if (ast->type == NODE_COMMAND)
    {
        struct ast_command *a = ast->data.ast_command;
        for (int j = 0; j < a->count; j++)
        {
            if (a->argv[j] == NULL)
                break;
            for (int index = 0; index < (int)strlen(a->argv[j]); index++)
            {
                if (a->argv[j][index] == '$')
                {
                    char *new = transform_char(a->argv[j], env, &index);
                    a->argv[j] = new;
                }
                if (a->argv[j] == NULL)
                    break;
            }
        }
        return;
    }
    // printf("transform command2\n");
    return;
}

void concat_node(struct ast *node1, struct ast *node2,
                 struct environnement *env)
{
    // printf("concat node\n");
    if (node1->type == NODE_COMMAND && node2->type == NODE_SIMPLE_QUOTE)
    {
        struct ast_command *a1 = node1->data.ast_command;
        struct ast_simple_quote *a2 = node2->data.ast_simple_quote;
        char **res = malloc(sizeof(char *) * (1 + a1->count));
        int index = 0;
        for (int a = 0; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit"))
            {
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }
        }
        if (a2->argv != NULL)
        {
            res[index] = strndup(a2->argv, strlen(a2->argv));
            a1->count += 1;
        }
        free(a1->argv);
        a1->argv = res;
    }
    else if (node1->type == NODE_COMMAND && node2->type == NODE_DOUBLE_QUOTE)
    {
        struct ast_double_quote *a2 = node2->data.ast_double_quote;
        struct ast_command *a1 = node1->data.ast_command;
        char **res = malloc(sizeof(char *) * (1 + a1->count));
        int index = 0;
        for (int a = 0; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit"))
            {
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }
        }
        if (a2->argv != NULL)
        {
            res[index] = strndup(a2->argv, strlen(a2->argv));
            a1->count += 1;
        }
        free(a1->argv);
        a1->argv = res;
    }
    else if (node1->type == NODE_COMMAND && node2->type == NODE_COMMAND)
    {
        struct ast_command *a1 = node1->data.ast_command;
        struct ast_command *a2 = node2->data.ast_command;
        char **res = malloc(sizeof(char *) * (a1->count + a2->count));
        int index = 0;
        int a = 0;
        for (; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit"))
            {
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }
        }
        for (a = 0; a < a2->count; a++)
        {
            if (strcmp(a2->argv[a], "exit"))
            {
                if (a2->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            if (a2->argv[a] != NULL)
            {
                res[index] = strndup(a2->argv[a], strlen(a2->argv[a]));
                index += 1;
                a1->count += 1;
            }
        }
        free(a1->argv);
        a1->argv = res;
    }
}

void concat_command(struct ast_main_root *a, int *i, struct environnement *env)
{
    // print_variables(env);
    if (env == NULL)
        fprintf(stderr, "concat_command");
    int inter = *i;
    // printf("child = %d\n", a->nb_children);
    // printf("inter = %d\n", inter);
    int ii = *i;
    transform_command(a->children[inter], env);
    if (is_nullf(a->children[inter]))
    {
        return;
    }
    // printf("coucou1\n");
    inter += 1;
    // printf("inter = %d\n", inter);
    for (; inter < a->nb_children; inter++)
    {
        // printf("inter 1 = %d\n", inter);
        if (a->children[inter]->type == NODE_DOUBLE_QUOTE
            || a->children[inter]->type == NODE_SIMPLE_QUOTE
            || a->children[inter]->type == NODE_COMMAND)
        {
            // printf("inter 2 = %d\n", inter);
            transform_command(a->children[inter], env);
            // printf("concat node\n");
            concat_node(a->children[ii], a->children[inter], env);
            *i = inter;
            // printf("inter 3 = %d\n", inter);
            if (is_nullf(a->children[inter]))
            {
                return;
            }
            // printf("inter 5 = %d\n", inter);
        }
    }
    return;
}

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

int exec_ast_if(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_if *a = ast->data.ast_if;
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

int exec_ast_elif(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    // printf("elif\n");
    struct ast_elif *a = ast->data.ast_elif;
    int res = exec_ast(a->cond, env);
    if (res == 0)
    {
        exec_ast(a->then, env);
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
    struct ast_pipe *a = ast->data.ast_pipe;
    printf("%p ", (void *)a);
    // struct ast *left = a->left;
    // struct ast *right = a->right;
    // return pipe_exec(left->data.ast_command->argv,
    // left->data.ast_command->count, right->data.ast_command->argv,
    // right->data.ast_command->count);
    return 1;
}

int exec_ast_command(struct ast *ast, struct environnement *env)
{
    if (env == NULL)
        return 1;
    if (env->exit_status != -1)
        return env->exit_status;
    struct ast_command *a = ast->data.ast_command;
    char **tab = get_all_var(a->argv[0]);
    if (tab != NULL)
    {
        insert_variable(tab[0], tab[1], env);
        free(tab);
        return 0;
    }
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
    /*case BREAK_COMMAND:
        return_value = command_break(env);
    case EXIT_COMMAND:
        return_value = command_exit(a->argv, a->count, env);*/
    default:
        return_value = command_exec(a->argv, a->count, env);
    }
    if (env->exit_status != -1)
        return env->exit_status;
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
    if (env->exit_status != -1)
        return env->exit_status;
    return res;
}
