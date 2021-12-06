#ifndef COMMAND_REDIR_H
#define COMMAND_REDIR_H

int command_redir_r(char *command[], int count, char *file);
int command_redir_l(char *command[], int count, char *file);
int command_redir_rr(char *command[], int count, char *file);
int command_redir_lr(char *command[], int count, char *file);
int command_redir_l_and(int out, int in);
int command_redir_r_and(int out, int in);
int command_redir_r_pipe(char *command[], int count, char *file);

#endif
