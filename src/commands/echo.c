#include <stdio.h>
#include <string.h>

int echo(char const *argv[])
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
    for (; argv[i]; i++)
    {
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
        if (argv[i + 1] != NULL)
            printf(" ");
    }
    if (flag < 2)
        printf("\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc == 8)
    {
        printf("haha je t'ais fais une blague\n");
    }
    return echo(argv);
    return 0;
}
