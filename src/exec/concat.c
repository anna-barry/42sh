#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "../functionnal/functionnal.h"
#include "../parser/parser.h"

char *itoa(int value, char *s)
{
    int l = 0;
    int a = value;
    int neg = 0;
    if (value < 0)
    {
        l += 1;
        a *= -1;
        neg = 1;
        value = a;
    }
    while (a >= 10)
    {
        l += 1;
        a = a / 10;
    }
    s[l + 1] = '\0';
    for (int i = l; i >= 0; i--)
    {
        s[i] = (value % 10) + '0';
        value = value / 10;
    }
    if (neg == 1)
        s[0] = '-';
    return s;
}

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
    char *tr_char = NULL;
    if (strcmp(indice, "UID") == 0)
    {
        char *a = malloc(sizeof(char) * 15);
        tr_char = itoa((int)(env->uid), a);
        // free(a);
    }
    else
    {
        struct variable *inter = env->var;
        while (inter)
        {
            // printf("inter is %s\n", inter->name);
            if (strcmp(indice, inter->name) == 0)
                break;
            inter = inter->next;
        }
        if (inter == NULL)
        {
            char *res = NULL;
            free(argv);
            free(indice);
            return res;
        }
        tr_char = inter->value;
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
    for (unsigned long f = 0; f < strlen(tr_char); f++)
    {
        res[avance] = tr_char[f];
        avance += 1;
        if (avance >= cap)
        {
            cap *= 2;
            res = realloc(res, sizeof(char) * cap);
        }
    }
    avance += 1;
    a += strlen(indice) + 1;
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
    *index += (strlen(tr_char) - strlen(indice));
    free(argv);
    free(indice);
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
        // char **res = malloc(sizeof(char *) * (2 + a1->count));
        // res[a1->count + 1] = NULL;
        // int index = 0;
        for (int a = 0; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit") == 0)
            {
                // printf("in if\n");
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            /*if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                //free(a1->argv[a]);
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }*/
        }
        if (a2->argv != NULL)
        {
            // res[index] = strndup(a2->argv, strlen(a2->argv));
            a1->argv[a1->count] = strndup(a2->argv, strlen(a2->argv));
            // free(a2->argv);
            a1->count += 1;
        }
        // free(a1->argv);
        //   ******  Test ******  //
        /*
        a1->argv = malloc(sizeof(char *) * (2 + a1->count));
        a1->count = index;
        for (int i = 0; i < index; i++)
        {
            if (a1->argv[i])
            {
                free(a1->argv[i]);
            }

            if (res[i])
            {
                a1->argv[i] = strndup( res[i], strlen(res[i]));
               // free(res[i]);
            }
        }
        for (int i = 0; i < 2 + a1->count; i++)
        {
            if (res[i])
            {
                free(res[i]);
            }
        }

        free(res);*/
        // a1->argv = res;
    }
    else if (node1->type == NODE_COMMAND && node2->type == NODE_DOUBLE_QUOTE)
    {
        struct ast_double_quote *a2 = node2->data.ast_double_quote;
        struct ast_command *a1 = node1->data.ast_command;
        // char **res = malloc(sizeof(char *) * (2 + a1->count));
        // a1->argv = realloc(a1->argv, sizeof(char *) * (2 + a1->count));
        // res[a1->count + 1] = NULL;
        a1->argv[a1->count + 1] = NULL;
        // int index = 0;
        for (int a = 0; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit") == 0)
            {
                // printf("in if\n");
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                return;
            }
            /*if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                free(a1->argv[a]);
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }*/
        }
        // free(a1->argv);
        if (a2->argv != NULL)
        {
            // res[index] = strndup(a2->argv, strlen(a2->argv));

            a1->argv[a1->count] = strndup(a2->argv, strlen(a2->argv));
            // free(a2->argv);
            a1->count += 1;
        }
        //  ******  Test ******
        // free(a1->argv);
        // a1->argv = malloc(sizeof(char *) * (2 + a1->count));
        /*
        a1->count = index;
        for (int i = 0; i < index; i++)
        {
            if (a1->argv[i])
            {
                free(a1->argv[i]);
            }

            if (res[i])
            {
                a1->argv[i] = strndup( res[i], strlen(res[i]));
               // free(res[i]);
            }
        }
        for (int i = 0; i < 2 + a1->count; i++)
        {
            if (res[i])
            {
                free(res[i]);
            }
        }

        free(res);*/
        // a1->argv = res;
    }
    else if (node1->type == NODE_COMMAND && node2->type == NODE_COMMAND)
    {
        struct ast_command *a1 = node1->data.ast_command;
        struct ast_command *a2 = node2->data.ast_command;
        // char **res = malloc(sizeof(char *) * (a1->count + a2->count + 1));
        a1->argv =
            realloc(a1->argv, sizeof(char *) * (a1->count + a2->count + 1));
        // res[a1->count + a2->count] = NULL;
        a1->argv[a1->count + a2->count] = NULL;
        int index = 0;
        int a = 0;
        /*for (; a < a1->count; a++)
        {
            if (strcmp(a1->argv[a], "exit"))
            {
                printf("in if\n");
                if (a1->count > a + 1 && a1->argv[a + 1] != NULL)
                    env->exit_status = atoi(a1->argv[a + 1]);
                else
                    env->exit_status = 0;
                free(res);
                return;
            }
            if (a1->argv[a] != NULL)
            {
                res[index] = strndup(a1->argv[a], strlen(a1->argv[a]));
                free(a1->argv[a]);
                index += 1;
            }
            else
            {
                a1->count -= 1;
            }
        }
        free(a1->argv);*/
        index = a1->count;
        for (a = 0; a < a2->count; a++)
        {
            if (a2->argv[a] && strcmp(a2->argv[a], "exit") == 0)
            {
                // printf("in if\n");
                if (a2->count > a + 1 && a2->argv[a + 1] != NULL)
                    env->exit_status = atoi(a2->argv[a + 1]);
                else
                    env->exit_status = 0;
                // free(res);
                return;
            }
            if (a2->argv[a] != NULL)
            {
                // res[index] = strndup(a2->argv[a], strlen(a2->argv[a]));
                a1->argv[index] = strndup(a2->argv[a], strlen(a2->argv[a]));
                // free(a2->argv[a]);
                index += 1;
                a1->count += 1;
            }
        }
        // free(a2->argv);
        // a1->argv = res;
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
    // transform_command(a->children[inter], env);
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
            // transform_command(a->children[inter], env);
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
