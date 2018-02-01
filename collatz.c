#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void collatz(int n) {
  if (n == 1) {
    printf("%d\n", n);
  } else {
    printf("%d, ", n);
    if (n % 2 == 0) {
      return collatz(n / 2);
    } else {
      return collatz(3 * n + 1);
    }
  }
}

int main(int argc, char const *argv[]) {

  int startNum = atoi(argv[1]);

  if (argc != 2 || startNum < 0) {
    printf("Please only supply one positive number as an argument!\n");
    return 1;
  }

  pid_t pid;
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "ForkFailed\n");
    return 1;
  } else if (pid == 0) {
    printf("Child Process Executing\n");
    collatz(startNum);
  } else {
    wait(NULL);
    printf("Child Complete\n");
  }
  return 0;
}
