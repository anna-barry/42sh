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

int is_dot(char *c)
{
    return c[0] == '.';
}

char *get_input(char *path)
{
    FILE * fptr = fopen (path, "r");
    size_t cap = 300;
    char *input = malloc(cap * sizeof(char));
    size_t index = 0;
	char str1 = fgetc(fptr);
    // If there's a shebang
    if (str1 == '#')
    {
        while (str1 != EOF && str1 != '\n')
        {
            str1 = fgetc(fptr);
        }
    }
	while (str1 != EOF)
	{
        if (index + 1 >= cap)
        {
            cap *= 2;
            input = realloc(input, cap * sizeof(char));
        }
        input[index++] = str1;
		str1 = fgetc(fptr);
	}
    fclose (fptr);
    return input;
}

int my_dot(char *argv[], int count, struct environnement *env)
{
   // if (is_dot(argv[0]))
   // {
    // Get path 
    // ./test.sh else
    // . test.sh if
    int res_e = 0;

    if (count > 0 && strcmp(argv[0], ".") == 0)
    { 
        const char *input = (const char *)get_input(argv[1]);
        struct lexer *lexer = lexer_new(input);
        struct ast *ast = build_ast(lexer, NODE_ROOT);
        res_e = execution(ast, env);
        token_free(lexer_pop(lexer));
        lexer_free(lexer);
        my_pretty_free(ast);
        //free((void *)input);
    }
    else
    {   
        printf("in else with (%s)\n", argv[0]);
        const char *input = (const char *)get_input(argv[0]);
        struct lexer *lexer = lexer_new(input);
        struct ast *ast = build_ast(lexer, NODE_ROOT);
        struct environnement *env2 = init_env();
        res_e = execution(ast, env2);
        token_free(lexer_pop(lexer));
        lexer_free(lexer);
        my_pretty_free(ast);
        free_environnement(env2);
        //free((void *)input);
    }
    //}
    return  res_e;
}
