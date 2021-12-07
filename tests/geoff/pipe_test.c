#include <err.h>
#include <string.h>

#include "../../src/commands/command_pipe.h"

int main(int argc, char const *argv[])
{
    char *argv0_left[2] = { "ls" };
    char *argv0_right[3] = { "cat", "-e" };

    char *argv1_left[3] = { "echo", "Hello" };
    char *argv1_right[3] = { "cat", "-e" };

    char *argv2_left[3] = { "echo", "Hallo" };
    char *argv2_right[4] = { "tr", "a", "e" };

    char *argv3_left[3] = { "echo", "a" };
    char *argv3_right[3] = { "echo", "b" };

    char *argv4_left[2] = { "ls" };
    char *argv4_right[4] = { "tr", "a", "e" };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./pipe test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return pipe_exec(argv0_left, argv0_right, 1, 2);
        else if (strcmp("test1", argv[1]) == 0)
            return pipe_exec(argv1_left, argv1_right, 2, 2);
        else if (strcmp("test2", argv[1]) == 0)
            return pipe_exec(argv2_left, argv2_right, 2, 3);
        else if (strcmp("test3", argv[1]) == 0)
            return pipe_exec(argv3_left, argv3_right, 2, 2);
        else if (strcmp("test4", argv[1]) == 0)
            return pipe_exec(argv4_left, argv4_right, 1, 3);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
