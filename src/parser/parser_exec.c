#include "parser_exec.h"

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/echo.h"
#include "parser.h"

int exec_ast_if_root(struct ast *ast)
{
  struct ast_if_root *a = ast->data.ast_if_root;
  for (int i = 0; i < a->nb_children; i++)
    {
      a->status = exec_ast(a->children[i]);
      if (a->status == 0)
	break;
    }
  return a->status;
}

void exec_ast_root(struct ast *ast)
{
    struct ast_main_root *a = ast->data.ast_main_root;
    int res = -1;
    for (int i = 0; i < a->nb_children; i++)
    {
        res = exec_ast(a->children[i]);
    }
    return res;
}

int exec_ast_if(struct ast *ast)
{
  struct ast_if *a = ast->data.ast_if;
  int res = -1;
  int i = 0;
  for (int i = 0; i < a->count_cond; i++)
    {
      if ((res = exec_ast(a->cond[i])) == 0)
	i = exec_ast(a->then);
    }
  return res;
}

int exec_ast_elif(struct ast *ast)
{
  struct ast_if *a = ast->data.ast_elif;
  int res = -1;
  int i = 0;
  for (int i = 0; i < a->count_cond; i++)
    {
      if ((res = exec_ast(a->cond[i])) == 0)
	i = exec_ast(a->then);
    }
  return res;
}

int exec_ast_else(struct ast *ast)
{
    struct ast_else *a = ast->data.ast_else;
    return exec_ast(a->then);
}

int exec_ast_command(struct ast *ast)
{
    struct ast_command *a = ast->data.ast_command;
    for (size_t i = 0; i < a->count; i++)
    {
        if (strcmp("echo", argv[0]) == 0)
            return echo(a->cond[i]);
        else
            return command_exec(a->cond[i]);
    }
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
    return ast_exec[ast->type](ast);
}

void execution(struct ast_main_root *ast)
{
    return exec_ast(&ast.type);
}
