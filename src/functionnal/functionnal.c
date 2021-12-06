#include "functionnal.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

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
            low1 = (char) ((int) low1 + 32);
        char low2 = s2[i];
        if (low2 >= 'A' && low2 <= 'Z')
            low2 = (char) ((int) low2 + 32);
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

void insert_variable(char *name, char *value, struct environnement *new)
{
    struct variable *new_v = malloc(sizeof(struct variable));
    if (!new_v)
        err(2, "Error with malloc\n");
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

struct environnement *init_env(void)
{
    struct environnement *env = malloc(sizeof(struct environnement));
    env->nb_variables = 0;
    env->var = NULL;
    // ADD RANDOM VARIABLE
    insert_variable("RANDOM", NULL, env);
    // ADD UID VARIABLE
    env->uid = getuid();
    // ADD OLDPWD VARIABLE
        // TODO
    // ADD IFS VARIABLE
    insert_variable("IFS", "\n", env);
    //print_variables(env);
    return env;
}
