#ifndef COMMAND_H
#define COMMAND_H
#include "../functionnal/functionnal.h"

int command_exit(char *command[], int count, struct environnement *env);

int command_break(struct environnement *env);

int command_continue(struct environnement *env);

int command_exec(char *argv[], int count);

#endif
