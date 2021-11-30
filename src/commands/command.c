#include "command.h"

#include <err.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int command_exec(char *argv[]) // the command should be as : char *argv[4] = {
                               // "echo", "geoffroy", "geoffroy", NULL };
                               // the NULL argument is nessessary.
{
    int pid = fork();
    if (pid == -1)
        err(1, NULL);
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1) // execute the commande
            return 127;
    }
    int wstatus;
    if (waitpid(pid, &wstatus, 0) == -1)
        err(1, NULL);
    return WEXITSTATUS(wstatus); // return the return value of the command
}

// int main()
// {
//     char *argv[4] = { "echo", "geoffroy", "geoffroy", NULL };
//     return command_exec(argv);
//     return 0;
// }

// if (argc < 3) {
//     err(2, "Not enough arguments");
//   }
//   int fd = open(argv[1], O_CREAT | O_WRONLY, 0644);
//   int pid = fork();
//   char *nb2 = argv[2];
//   int i = 0;
//   for (; i + 2 < argc; i++) {
//     argv[i] = argv[i + 2];
//   }
//   for (; i < argc; i++) {
//     argv[i] = NULL;
//   }
//   if (pid == 0) {
//     if (dup2(fd, 1) == -1)
//       fprintf(stderr, "Error with dup2");
//     if (execvp(nb2, argv) == -1)
//       return 127;
//     //     return EXIT_FAILURE;
//   } else {
//     int wstatus;
//     int child_pid = waitpid(pid, &wstatus, 0);
//     if (child_pid == -1) {
//       errx(1, "wrong child pid \n");
//     }
//     if (WIFEXITED(wstatus)) {
//       if (WEXITSTATUS(wstatus) == 127) {
//         fprintf(stderr, "Incorrect command");
//         return 1;
//       }
//     }
//     printf("%s exited with %i!\n", nb2, WEXITSTATUS(wstatus));
//   }
//   close(fd);
//   return 0;
// }
