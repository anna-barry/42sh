int main()
{
    char *argv[2] = { "echo", "geoffroy" };
    char *file = "tester.txt";
    printf("recieve\n");
    // fprintf(fd, "new file descriptor\n");
    // command_redir_r(argv, 2, file);
    command_redir_r_pipe(argv, 2, file);
    // fprintf(fd, "old file descriptor\n");
    printf("lounch\n");
    return 0;
}
