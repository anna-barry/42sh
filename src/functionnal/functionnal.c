#include "functionnal.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_variables(struct environnement *new)
{
    struct variable *index = new->var;
    for (; index != NULL; index = index->next)
    {
        if (index->value == NULL)
        {
            printf("(%s) = NULL\n", index->name);
        }
        else
            printf("(%s) = (%s)\n", index->name, index->value);
    }
}

// S > S2 -> 1
// S2 > S -> -1
// S = S2 -> 0
int my_strcmp(char *s, char *s2)
{
    size_t len1 = strlen(s);
    size_t len2 = strlen(s2);
    for (size_t i = 0; i < len1 && i < len2; i++)
    {
        char low1 = s[i];
        if (low1 >= 'A' && low1 <= 'Z')
            low1 = (char)((int)low1 + 32);
        char low2 = s2[i];
        if (low2 >= 'A' && low2 <= 'Z')
            low2 = (char)((int)low2 + 32);
        if (low1 < low2)
        {
            return -1;
        }
        else if (low1 > low2)
        {
            return 1;
        }
    }
    if (len1 < len2)
    {
        return -1;
    }
    else if (len2 > len1)
    {
        return 1;
    }
    return 0;
}

int find_variable(char *name, struct environnement *new)
{
    struct variable *index = new->var;
    for (; index != NULL; index = index->next)
    {
        if (strcmp(index->name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void free_variables(struct variable *index)
{
    if (index->next != NULL)
        free_variables(index->next);
    if (index)
        free(index);
}

void free_environnement(struct environnement *new)
{
    for (int i = 0; i < new->nb_args; i++)
    {
        if (new->args[i])
            free(new->args[i]);
        if (new->args_b[i])
            free(new->args_b[i]);
    }
    // if (new->args)
    //     free(new->args);
    // if (new->args_b)
    //     free(new->args_b);
    free_variables(new->var);
    if (new)
        free(new);
}

void update_variable(char *name, char *value, struct environnement *new)
{
    if (find_variable(name, new) == 0)
    {
        insert_variable(name, value, new);
    }
    else
    {
        struct variable *index = new->var;
        if (strcmp(index->name, name) == 0)
        {
            if (value == NULL)
            {
                new->var = new->var->next;
                if (index->name)
                    free(index->name);
                if (index->value)
                    free(index->value);
                if (index)
                {
                    free(index);
                }
            }
            else
                index->value = value;
        }
        else
        {
            for (; index->next != NULL; index = index->next)
            {
                if (strcmp(index->next->name, name) == 0)
                {
                    if (value == NULL)
                    {
                        struct variable *tmp = index->next;
                        index->next = index->next->next;
                        if (tmp->name)
                            free(tmp->name);
                        if (tmp->value)
                            free(tmp->value);
                        if (tmp)
                        {
                            free(tmp);
                        }
                    }
                    else
                        index->value = value;
                }
            }
        }
    }
}

void insert_variable(char *name, char *value, struct environnement *new)
{
    if (find_variable(name, new) == 1)
    {
        update_variable(name, value, new);
        return;
    }
    struct variable *new_v = malloc(sizeof(struct variable));
    if (!new_v)
        err(2, "Error with malloc\n");
    /* new_v->name = strndup(name, strlen(name));
     if (value == NULL)
     {
        new_v->value = NULL;
     }
     else
         new_v->value = strndup(value, strlen(value));*/
    new_v->name = name;
    new_v->value = value;
    if (new->nb_variables == 0)
    {
        new_v->next = NULL;
        new->var = new_v;
        new->nb_variables++;
        return;
    }
    struct variable *index = new->var;
    // index->name greater than name
    if (my_strcmp(index->name, name) == 1)
    {
        new_v->next = index;
        new->var = new_v;
    }
    else
    {
        // While name greater than index->name
        while (index->next != NULL && my_strcmp(index->next->name, name) == -1)
        {
            index = index->next;
        }
        struct variable *tmp = index->next;
        index->next = new_v;
        new_v->next = tmp;
    }
    new->nb_variables++;
}

int is_var(char *command)
{
    for (size_t i = 0; i < strlen(command); i++)
    {
        if (command[i] == '=')
        {
            return i;
        }
    }
    return -1;
}

char **get_all_var(char *command)
{
    // printf("%s is command \n", command);
    if (command == NULL)
    {
        return NULL;
    }

    int res = is_var(command);
    if (res == -1)
    {
        return NULL;
    }
    char **result = malloc(sizeof(char *) * 2);
    // char *first = strndup(command, res);
    // char *second = strndup(command + res + 1, strlen(command) - res);
    result[0] = strndup(command, res); // name
    result[1] = strndup(command + res + 1, strlen(command) - res); // value
    // printf("first is %s and second is %s \n", result[0], result[1]);
    return result;
}

struct environnement *init_env(void)
{
    struct environnement *env = malloc(sizeof(struct environnement));
    env->nb_variables = 0;
    env->var = NULL;
    // ADD RANDOM VARIABLE
    char *rand = strndup("RANDOM", strlen("RANDOM"));
    insert_variable(rand, NULL, env);
    // ADD UID VARIABLE
    env->uid = getuid();
    env->exit_status = -1;
    // ADD OLDPWD VARIABLE + PWD
    // TODO
    // ADD IFS VARIABLE
    char *ifs = strndup("IFS", strlen("IFS"));
    char *value_ifs = strndup("\n", strlen("\n"));
    insert_variable(ifs, value_ifs, env);
    // print_variables(env);
    return env;
}
