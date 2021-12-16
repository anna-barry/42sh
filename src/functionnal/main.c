#include <err.h>
#include <errno.h>
#include <getopt.h>
//#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "../parser/parser_printer.h"
#include "../parser/parser_free.h"
#include "functionnal.h"

int getnb(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (file != NULL)
    {
        int counter = 0;
        char c;
        while (fscanf(file, "%c", &c) != EOF)
        {
            if (counter < 123456789)
            {
                counter++;
            }
            else
                err(2, "ERROR in getnb\n");
        }
        fclose(file);
        return counter + 1;
    }
    else
        err(2, "ERROR in getnb\n");
    //fclose(file);
    return -1;
}

char *filetostring(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    int len = getnb(filepath);
    if (file != NULL)
    {
        char *ptr = malloc(sizeof(char) * len);
        if (ptr == NULL)
        {
            err(2, "ERROR in fileToString\n");
        }
        char c;
        int offset = 0;
        while (fscanf(file, "%c", &c) != EOF)
        {
            ptr[offset] = c;
            offset++;
        }
        fclose(file);
        if (ptr[offset - 1] == '\n')
          ptr[offset - 1] = '\0';
        else
          ptr[offset] = '\0';
        return ptr;
    }
    else
        err(2, "ERROR in fileToString \n");
    fclose(file);
    return NULL;
}

char *find_input(int argc, char *argv[])
{
    char *input = NULL;
    if (argc == 1)
    {
        input = malloc(sizeof(char) * 123456789);
        for (size_t i = 0; i < 123456789; i++)
        {
            input[i] = '\0';
        }
        scanf("%[^\n]", input);
        return input;
    }
    else if (argc == 2)
    {
        input = filetostring(argv[1]);
        return input;
    }
    static struct option long_options[] = {
        { "c_option", required_argument, 0, 'c' }, { 0, 0, 0, 0 }
    };

    int opt = 0;
    int c_option = -1;
    int c_index = 0;
    int index = 0;
    while ((opt = getopt_long(argc, argv, "abc:d:012", long_options, &index))
           != -1)
    {
        switch (opt)
        {
        case 'c':
            c_option = 1;
            c_index = optind - 1;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
        }
    }
    if (c_option)
    {
        int size = strlen(argv[c_index]);
        input = malloc(sizeof(char) * (size + 1));
        for (int i = 0; i < size; i++)
        {
            input[i] = argv[c_index][i];
        }
        input[size] = '\0';
        return input;
    }
    else
    {
        fprintf(stderr, "Error: nor script nor input \n");
    }
    return NULL;
}
/*
*
* Catching sigint
*
*

static void sigintHandler(int sig)
{
    printf("int sig %i \n", sig);
}
*/

int main(int argc, char *argv[])
{
    if (argc < 1)
        fprintf(stderr, "Number of arguments not correct");

   // signal(SIGINT, sigintHandler);

    int res_e = 2;
    int pretty_print = 0;
    if (argc > 0 && argv[1] != NULL && strcmp(argv[1], "--pretty-print") == 0)
    {
            pretty_print = 1;
            int i = 1;
            for (; i + 1 < argc; i++)
            {
                argv[i] = argv[i + 1];
            }
            argv[i] = "\0";
    }
    struct environnement *env = init_env();
    const char *input = (const char *)find_input(argc, argv);
    if (input == NULL || strlen(input) == 0)
      return 1;
    struct info_lexer *new = lexer_init();
    lexer_new(input, new);
    struct ast *ast = build_ast(new, NODE_ROOT);
    if (pretty_print == 1)
    {
        printf("______ start of pretty print ________ \n");
        my_pretty_print(ast);
        printf("\n ______ end of pretty print ________ \n");
    }
    res_e = execution(ast, env);
    token_free(lexer_pop(new->lexer));
    lexer_info_free(new);
    my_pretty_free(ast);
    //free_environnement(env);
    free((void *)input);
    free_environnement(env);
    return res_e;
}
