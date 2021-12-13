#include "cd.h"

#include <unistd.h>

int cd(char *path)
{
    if (chdir(path) == -1)
    {
        return 1;
    }
    return 0;
}
