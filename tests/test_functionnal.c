#include <stdio.h>

#include "../src/functionnal/functionnal.h"

int main(int argc, char *argv[])
{
    struct environnement *env = init_env();
    for (int i = 1; i + 1 < argc; i += 2)
    {
        insert_variable( argv[i], argv[i + 1], env);
    }
    print_variables(env);
    printf("\n\n\n\n");
    update_variable("anna", "updating", env);
    print_variables(env);
    free_environnement(env);
    return 0;
}
