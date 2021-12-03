#pragma once

#include <sys/types.h> 
#include <unistd.h> 

struct variable
{
    char *name;
    char *value;
};

struct environnement
{
    char **args; /// $*
    char **args_b; /// $@
    int nb_args; /// $#
    pid_t pid; /// $$
    int return_value; // $?
    struct variables *var;
    int nb_variables;
    int cap_variables;
};

struct environnement *init_env(void);

void insert_variable(char *name, char *value, struct environnement *new);