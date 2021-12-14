#ifndef COMMAND_H
#define COMMAND_H
#include "../functionnal/functionnal.h"

int command_exit(char *command[], int count, struct environnement *env);

int command_break(struct environnement *env);

int command_continue(struct environnement *env);

int simple_command_exec(char *argv[], int count);

int command_exec(char *argv[], int count, struct environnement *env);

int my_dot(char *argv[], int count, struct environnement *env);

int is_dot(char *c);

int my_unset(char *argv[], struct environnement *env);

#endif
