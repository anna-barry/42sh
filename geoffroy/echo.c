#include <stdio.h>
#include <string.h>

int echo_e(char const *argv[]) {
  for (int i = 2; argv[i]; i++) {
    for (int j = 0; argv[i][j]; j++) {
      if (argv[i][j + 1] != '\0' &&
          (argv[i][j] == '\\' && argv[i][j + 1] == 'n')) {
        printf("\n");
        j++;
      } else if (argv[i][j + 1] != '\0' &&
                 (argv[i][j] == '\\' && argv[i][j + 1] == 't')) {
        printf("\t");
        j++;
      } else if (argv[i][j + 1] != '\0' &&
                 (argv[i][j] == '\\' && argv[i][j + 1] == '\\')) {
        printf("\\");
        j++;
      } else
        printf("%c", argv[i][j]);
    }
    if (argv[i + 1] != NULL) {
      printf(" ");
    }
  }
  return 0;
}

int echo_n(char const *argv[]) {
  for (int i = 2; argv[i]; i++) {
    for (int j = 0; argv[i][j]; j++) {
      printf("%c", argv[i][j]);
    }
    if (argv[i + 1] != NULL) {
      printf(" ");
    }
  }
  return 0;
}

int simple_echo(char const *argv[]) {
  for (int i = 1; argv[i]; i++) {
    for (int j = 0; argv[i][j]; j++) {
      printf("%c", argv[i][j]);
    }
    if (argv[i + 1] != NULL) {
      printf(" ");
    }
  }
  printf("\n");
  return 0;
}

int echo(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("\n");
  } else if (strcmp("-n", argv[1]) == 0) {
    return echo_n(argv);
  } else if (strcmp("-e", argv[1]) == 0) {
    return echo_e(argv);
  } else {
    return simple_echo(argv);
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  return echo(argc, argv);
  return 0;
}
