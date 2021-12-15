#ifndef STRUCT_INIT_H
#define STRUCT_INIT_H

#include "struct.h"

struct ast_main_root *create_main_root();

struct ast_if_root *create_if_root();

struct ast_command *create_command();

struct ast_pipe *create_pipe();

struct ast_and *create_and();

struct ast_or *create_or();

#endif
