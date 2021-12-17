#include "echo.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

int test_null(char *argv[], int count)
{
    for (int i = 1; i < count; i++)
    {
        if (argv[i] != NULL)
            return 1;
    }
    return 0;
}

int echo(char *argv[], int count)
{
    // printf("count is = %d\n", count);
    // for (int i = 0; i < count; i++)
    //     printf("elt %d is = %s\n", i, argv[i]);
    if (test_null(argv, count) == 0)
    {
        printf("\n");
        fflush(stdout);
        return 0;
    }
    int flag_e = 0;
    int flag_n = 0;
    if (count == 2 && argv[1] == NULL)
    {
        return 0;
    }
    int begin = 1;
    int no_option = 1;
    for (int i = 1; (i < count) && no_option != 0; i++)
    {
        no_option = 0;
        if (argv[i] == NULL)
        {
            no_option = 1;
            begin++;
        }
        if (argv[i] != NULL && strcmp("-n", argv[i]) == 0)
        {
            flag_n = 1;
            no_option = 1;
            begin++;
        }
        if (argv[i] != NULL && strcmp("-e", argv[i]) == 0)
        {
            flag_e = 1;
            no_option = 1;
            begin++;
        }
        if (argv[i] != NULL
            && (strcmp("-en", argv[i]) == 0 || strcmp("-ne", argv[i]) == 0))
        {
            flag_e = 1;
            flag_n = 1;
            no_option = 1;
            begin++;
        }
    }
    int i = 1; // i = 1
    if (flag_e > 0 || flag_n > 0)
        i = begin;
    for (; i < count; i++)
    {
        // printf("\ncount %i is %s \n", i, argv[i]);
        if (argv[i] == NULL)
        {
            continue;
            // break;
        }
        // printf("%s\n", argv[i]);
        // printf("%c\n", argv[i][0]);
        for (int j = 0; j < (int)strlen(argv[i]); j++)
        {
            // printf("%c\n", argv[i][j]);
            if (flag_e == 1)
            {
                if (argv[i][j + 1] != '\0'
                    && (argv[i][j] == '\\' && argv[i][j + 1] == 'n'))
                {
                    printf("\n");
                    fflush(stdout);
                    j++;
                }
                else if (argv[i][j + 1] != '\0'
                         && (argv[i][j] == '\\' && argv[i][j + 1] == 't'))
                {
                    printf("\t");
                    fflush(stdout);
                    j++;
                }
                else if (argv[i][j + 1] != '\0'
                         && (argv[i][j] == '\\' && argv[i][j + 1] == '\\'))
                {
                    printf("\\");
                    fflush(stdout);
                    j++;
                }
                else
                {
                    printf("%c", argv[i][j]);
                    fflush(stdout);
                }
            }
            else
            {
                printf("%c", argv[i][j]);
                fflush(stdout);
            }
        }
        if (argv[i + 1] == NULL)
            continue;
        if (i != count - 1)
        {
            printf(" ");
            fflush(stdout);
        }
    }
    if (flag_n != 1)
    {
        printf("\n");
        fflush(stdout);
    }
    return 0;
}

// int main(int argc, char *argv[])
// {
//     if (argc == 0)
//     {
//         printf("I'm not sure about that one\n");
//     }
//     return echo(argv);
//     return 0;
// }

// int main()
// {
//     char *argv[4] = { "echo", "geoffroy", "geoffroy", NULL };
//     return echo(argv);
//     return 0;
// }

/*
int main(int argc, char const *argv[])
{
    char *argv0[3] = { "echo", "-n" };
    char *argv1[3] = { "echo", "-e" };
    char *argv2[4] = { "echo", "geoffroy", "geoffroy" };
    char *argv3[5] = { "echo", "-n", "-n", "geoff" };
    char *argv4[3] = { "echo", "geof\\nfroy" };
    char *argv5[6] = { "echo", "-n", "-n", "-e", "geof\\nfroy" };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./echo test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return echo(argv0, 2);
        else if (strcmp("test1", argv[1]) == 0)
            return echo(argv1, 2);
        else if (strcmp("test2", argv[1]) == 0)
            return echo(argv2, 3);
        else if (strcmp("test3", argv[1]) == 0)
            return echo(argv3, 4);
        else if (strcmp("test4", argv[1]) == 0)
            return echo(argv4, 2);
        else if (strcmp("test5", argv[1]) == 0)
            return echo(argv5, 5);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
*/
