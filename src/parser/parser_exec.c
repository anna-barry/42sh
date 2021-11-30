#include "parser_exec.h"

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/echo.h"
#include "parser.h"

void exec_ast_if_root(struct ast *ast)
{
    struct ast_if_root *a = (struct ast_if_root *)ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        exec_ast(&a->children.type);
    }
}

void exec_ast_root(struct ast *ast)
{
    struct ast_main_root *a = (struct ast_main_root *)ast;
    for (int i = 0; i < a->nb_children; i++)
    {
        exec_ast(&a->children.type);
    }
}

void exec_ast_if(struct ast *ast)
{
    struct ast_if *a = (struct ast_if *)ast;
    exec_ast(&a->cond.type);
    exec_ast(&a->then.type);
}

void exec_ast_elif(struct ast *ast)
{
    struct ast_elif *a = (struct ast_elif *)ast;
    exec_ast(&a->cond.type);
    exec_ast(&a->then.type);
}

void exec_ast_else(struct ast *ast)
{
    struct ast_else *a = (struct ast_else *)ast;
    exec_ast(&a->cond.type);
}

int exec_ast_command(struct ast *ast)
{
    struct ast_command *a = (struct ast_command *)ast;
    if (strcmp("echo", a->argv[0]))
        return echo(a->argv);
    else
        return command_exec(a->argv);
}

typedef void (*ast_exec_function)(struct ast *ast);

static ast_exec_function ast_exec[] =
{
    [NODE_IF] = exec_ast_if,
    [NODE_ELIF] = exec_ast_elif,
    [NODE_ELSE] = exec_ast_else,
    [NODE_COMMAND] = exec_ast_command,
    [NODE_IF_ROOT) = exec_ast_if_root,
    [NODE_ROOT] = exec_ast_root,
};

void exec_ast(struct ast *ast)
{
    ast_exec[ast->type](ast);
}

void execution(struct ast_main_root *ast)
{
    exec_ast(&ast.type);
}
