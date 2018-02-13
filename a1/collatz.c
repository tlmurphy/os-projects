#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

bool isInt(char const integer[]) {
  for (size_t i = 0; integer[i] != 0; i++) {
    if (!isdigit(integer[i])) {
      return false;
    }
  }
  return true;
}

void collatz(int n) {
  /**
  * Collatz Conjecture:
  * n = n/2 if n is even
  * n = 3 * n + 1 if n is odd
  */
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

  if (argc != 2 || !isInt(argv[1])) {
    puts("Please provide one positive integer argument!");
    return 1;
  }

  int startNum = atoi(argv[1]);

  if (startNum < 0) {
    puts("Please supply a positive number as an argument!");
    return 1;
  }

  pid_t pid;
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork Failed!\n");
    return 1;
  } else if (pid == 0) {
    collatz(startNum);
  } else {
    wait(NULL);
  }
  return 0;
}
