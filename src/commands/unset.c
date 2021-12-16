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

int my_export(char *argv[], struct environnement *env)
{
    
    char **res = get_all_var(argv[1]);
    
    if(res == NULL)
    {
        char * env_res = (char *)getenv(argv[1]);
        if (env_res != NULL)
        {
            return 0;
        }
        struct variable *index = env->var;
        for (; index != NULL; index = index->next)
        {
            if (strcmp(argv[1], index->name) == 0)
            {
                return setenv( (const char *)index->name,(const char *)index->value, 1);
            }
        }
    }
    else
    {
        return setenv( (const char *)res[0],(const char *)res[1], 1);
    }
    return 0;
}
