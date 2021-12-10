#ifndef FUNCTIONNAL_H
#define FUNCTIONNAL_H

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct variable
{
    char *name;
    char *value;
    struct variable *next;
};

struct environnement
{
    char **args; /// $*
    char **args_b; /// $@
    int nb_args; /// $#
    pid_t pid; /// $$
    int return_value; // $?
    int flag_loop_break; // = 0; // break command
    int flag_loop_continue; // = 0; // continue command
    int exit_status; // = -1; // exit command
    struct variable *var;
    uid_t uid;
    int nb_variables;
};

/*
 * Init function for environnement variables
 */
struct environnement *init_env(void);

void free_environnement(struct environnement *new);

/*
 * Print environnement variables
 */
void print_variables(struct environnement *new);

/*
 * Find environnement variables
 */
int find_variable(char *name, struct environnement *new);

/*
 * Update environnement variables
 */
void update_variable(char *name, char *value, struct environnement *new);

/*
 * Command is a new variable
 */
int is_var(char *command);

/*
 * Get char** for new variable
 */
char **get_all_var(char *command);

/*
 * Insert environnement variables
 */
void insert_variable(char *name, char *value, struct environnement *new);

#endif /* !FUNCTIONNAL_H */
