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

struct ast_if_root
{
    int nb_children;
    int status;
    enum ast_type *type;
    union ast_data *children;
};

struct ast_command
{
    int count;
    char **argv;
};

struct ast_if
{
    struct ast_command *cond;
    enum ast_type type;
    union ast_data *then;
};
 
struct ast_else
{
    enum ast_type type;
    union ast_data *then;
};
 
struct ast_elif
{
    struct ast_command *cond;
    enum ast_type type;
    union ast_data *then;
};

struct ast_main_root
{
    int nb_children;
    enum ast_type *type;
    union ast_data *children;
};

union ast_data
{
    struct ast_if_root *ast_if_root;
    struct ast_command *ast_command;
    struct ast_if *ast_if;
    struct ast_elif *ast_elif;
    struct ast_else *ast_else;
};
#endif
