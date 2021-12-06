#include <err.h>
#include <string.h>

#include "../../src/commands/command_pipe.h"

int main(int argc, char const *argv[])
{
    char *argv0_left[1] = { "ls" };
    char *argv0_right[2] = { "cat", "-e" };
    char *argv1_left[1] = { "echo", "Hello" };
    char *argv1_right[2] = { "cat", "-e" };
    char *argv2_left[1] = { "echo", "Hallo" };
    char *argv2_right[2] = { "tr", "a", "e" };
    char *argv3_left[1] = { "echo", "a" };
    char *argv3_right[2] = { "echo", "b" };
    char *argv4_left[1] = { "ls" };
    char *argv4_right[3] = { "tr", "a", "e" };
    char *argv4_left[1] = { "ls" };
    char *argv4_right[2] = { "tr", "a", "e" };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./pipe test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return echo(argv0_left, argv0_right, 1, 2);
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
