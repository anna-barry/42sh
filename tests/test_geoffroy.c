#include <stddef.h>

#include "../src/commands/command.h"

int main()
{
    printf("this is the test of an exec command\n", );
    char *argv[3] = { "echo", "geoffroy", NULL };
    return command_exec(argv);
    return 0;
}
