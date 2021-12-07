#include "echo.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

int echo(char *argv[], int count)
{
    int flag = 0;
    if (argv[1] == NULL)
    {
        printf("\n");
        return 0;
    }
    else if (strcmp("-n", argv[1]) == 0)
        flag = 2;
    else if (strcmp("-e", argv[1]) == 0)
        flag = 1;
    int i = 1;
    if (flag > 0)
        i = 2;
    for (; i < count; i++)
    {
        if (argv[i] == NULL)
        {
            break;
        }
        for (int j = 0; argv[i][j]; j++)
        {
            if (flag == 1)
            {
                if (argv[i][j + 1] != '\0'
                    && (argv[i][j] == '\\' && argv[i][j + 1] == 'n'))
                {
                    printf("\n");
                    j++;
                }
                else if (argv[i][j + 1] != '\0'
                         && (argv[i][j] == '\\' && argv[i][j + 1] == 't'))
                {
                    printf("\t");
                    j++;
                }
                else if (argv[i][j + 1] != '\0'
                         && (argv[i][j] == '\\' && argv[i][j + 1] == '\\'))
                {
                    printf("\\");
                    j++;
                }
                else
                    printf("%c", argv[i][j]);
            }
            else
                printf("%c", argv[i][j]);
        }
        if (i != count - 1)
        {
            printf(" ");
        }
    }
    if (flag < 2)
        printf("\n");
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
    char *argv0[2] = { "echo", NULL };
    char *argv1[3] = { "echo", "geoffroy", NULL };
    char *argv2[4] = { "echo", "geoffroy", "geoffroy", NULL };
    char *argv3[4] = { "echo", "-n", "geoffroy", NULL };
    char *argv4[3] = { "echo", "geof\\nfroy", NULL };
    char *argv5[4] = { "echo", "-e", "geof\\nfroy", NULL };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./echo test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return echo(argv0);
        else if (strcmp("test1", argv[1]) == 0)
            return echo(argv1);
        else if (strcmp("test2", argv[1]) == 0)
            return echo(argv2);
        else if (strcmp("test3", argv[1]) == 0)
            return echo(argv3);
        else if (strcmp("test4", argv[1]) == 0)
            return echo(argv4);
        else if (strcmp("test5", argv[1]) == 0)
            return echo(argv5);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
*/
