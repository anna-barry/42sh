#include <err.h>
#include <string.h>

#include "../../src/commands/command_redir.h"

int main(int argc, char const *argv[])
{
    char *argv0_left[2] = { "echo", "geoffroy" };
    char *argv1_left[2] = { "echo" };
    char *argv2_left[3] = { "echo", "Hallo" };
    char *argv3_left[3] = { "echo", "Hallo" };
    char *argv4_left[3] = { "echo", "geoffroy" };
    char *file = "tester.txt";
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./redir test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return command_redir_r_pipe(argv0_left, 2, file);
        else if (strcmp("test1", argv[1]) == 0)
            return command_redir_l(argv1_left, 1, file);
        else if (strcmp("test2", argv[1]) == 0)
            return command_redir_r_pipe(argv2_left, 2, file);
        else if (strcmp("test3", argv[1]) == 0)
            return command_redir_r(argv3_left, 2, file);
        else if (strcmp("test4", argv[1]) == 0)
            return command_redir_rr(argv4_left, 2, file);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
