#include <err.h>
#include <string.h>

#include "../../src/commands/echo.h"

int main(int argc, char const *argv[])
{
    char *argv0[2] = { "echo" };
    char *argv1[3] = { "echo", "geoffroy" };
    char *argv2[4] = { "echo", "geoffroy", "geoffroy" };
    char *argv3[4] = { "echo", "-n", "geoffroy" };
    char *argv4[3] = { "echo", "geof\\nfroy" };
    char *argv5[4] = { "echo", "-e", "geof\\nfroy" };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./echo test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return echo(argv0, 1);
        else if (strcmp("test1", argv[1]) == 0)
            return echo(argv1, 2);
        else if (strcmp("test2", argv[1]) == 0)
            return echo(argv2, 3);
        else if (strcmp("test3", argv[1]) == 0)
            return echo(argv3, 3);
        else if (strcmp("test4", argv[1]) == 0)
            return echo(argv4, 2);
        else if (strcmp("test5", argv[1]) == 0)
            return echo(argv5, 3);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
