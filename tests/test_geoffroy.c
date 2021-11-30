#include <stddef.h>

#include "../src/commands/command.h"

int main()
{
    char *argv[3] = { "echo", "geoffroy", NULL };
    return command_exec(argv);
    return 0;
}
