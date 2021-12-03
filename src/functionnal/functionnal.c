#include "functionnal.h"

#include <stdio.h>
#include <string.h>

void tri_all(struct environnement *new, int i)
{
    for (int j = new->nb_variables; j > i; j--)
    {
        if( j < new->nb_variables)
        {
            char *tmp = new->var[j].name;
            new->var[j].name = strndup(new->var[j - 1].name, strlen(new->var[j - 1].name));
            if (tmp)
                free(tmp);
            char *tmp2 = new->var[j].value;
            if (new->var[j - 1].value != NULL)
            {
                new->var[j].value = strndup(new->var[j - 1].value, strlen(new->var[j - 1].value));
            }
            else
                new->var[j].value = NULL;
            if (tmp2)
                    free(tmp2);

        }
        else
        {
            new->var[j].name = strndup(new->var[j - 1].name, strlen(new->var[j - 1].name));
            if (new->var[j - 1].value != NULL)
            {
                new->var[j].value = strndup(new->var[j - 1].value, strlen(new->var[j - 1].value));
            }
            else
                new->var[j].value = NULL;
        }
    }
}

void insert_variable(char *name, char *value, struct environnement *new)
{
    int i = 0;
    while( i < new->nb_variables && strcmp(new->var[i].name < name, name) < 0)
    {
        i++;
    }
    if (new->nb_variables + 1 >= new->cap_variables)
    {
        new->cap_variables *= 2;
        new->var = realloc(new->var, new->cap_variables * sizeof(struct variable));
    }
    tri_all(new, i);
    new->var[i].name = name;
    new->var[i].value = value;
}

struct environnement *init_env(void)
{
    struct environnement *env = malloc(sizeof(struct environnement));
    env->cap_variables = 200;
    env->nb_variables = 0;
    env->var = malloc(200 * sizeof(struct variable));
    return env;
}