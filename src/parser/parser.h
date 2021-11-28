#ifndef PARSER_H
#define PARSER_H

enum ast_type
{
    NODE_IF,
    NODE_ELIF,
    NODE_ELSE,
    NODE_ROOT,
    NODE_COMMAND,
};

struct ast_root
{
    int nb_children;
    int status;
    union ast_data *children;
};

struct ast_command
{
    char **argv;
};

struct ast_if
{
    struct ast_command *cond;
    union ast_data *then;
};
 
struct ast_else
{
    struct ast_node *cond;
};
 
struct ast_elif
{
    struct ast_node *cond;
    union ast_data *then;
};

struct ast_main_root
{
    int nb_children;
    union ast_data *children;
};

union ast_data
{
    struct ast_root *ast_root;
    struct ast_command *ast_command;
    struct ast_if *ast_if;
    struct ast_elif *ast_elif;
    struct ast_else *ast_else;
};
#endif
