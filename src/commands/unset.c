#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <ctype.h>


#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "../parser/parser_printer.h"
#include "../parser/parser_free.h"
#include "../functionnal/functionnal.h"

int my_unset(char *argv[], struct environnement *env)
{
    if (find_variable(argv[1], env))
    {
        update_variable(argv[1], NULL, env);
    }
    return 0;
}
