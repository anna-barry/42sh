#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getnb(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (file != NULL)
    {
        int counter = 0;
        char c;
        while (fscanf(file, "%c", &c) != EOF)
        {
            if (counter < INT_MAX)
            {
                counter++;
            }
            else
                fprintf(stderr, "ERROR in getnb");
        }
        return counter + 1;
    }
    else
        fprintf(stderr, "ERROR in getnb");
    fclose(file);
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
            fprintf(stderr, "ERROR in fileToString");
        }
        char c;
        int offset = 0;
        while (fscanf(file, "%c", &c) != EOF)
        {
            ptr[offset] = c;
            offset++;
        }
        fclose(file);
        ptr[offset] = '\0';
        return ptr;
    }
    else
        fprintf(stderr, "ERROR in fileToString");
    fclose(file);
    return NULL;
}

char *find_input(int argc, char *argv[])
{
    char *input = NULL;
    if (argc == 1)
    {
        input = malloc(sizeof(char) * INT_MAX);
        printf("Veuillez saisir l'input : \n");
        fflush(stdout);
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
        fprintf(stderr, "error");
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 1 || argc > 3)
        fprintf(stderr, "Number of arguments not correct");

    int pretty_print = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--pretty-print"))
        {
            pretty_print = 1;
        }
    }
    const char *input = (const)find_input(argc, argv);
    return 0;
}
