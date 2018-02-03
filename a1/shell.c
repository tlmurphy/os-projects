#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 80 // The maximum length command
#define MAX_SIZE MAX_LINE / 2 + 1

int main(int argc, char const *argv[]) {
  char input[MAX_SIZE];
  char * args[MAX_SIZE]; // command line arguments
  char * token;
  int bgFlag = 0;

  int shouldRun = 1; // flag to determine when to exit program
  while (shouldRun) {
    memset(args, 0, sizeof(args)); // Clear input before retrieving it again

    printf("osh> ");
    fflush(stdout);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Strip newline from input since it is a pain

    if (strcmp(input, "exit") == 0) {
      shouldRun = 0;
      continue;
    }

    token = strtok(input, " ");
    int i = 0;
    while (1) {
      if (token == NULL) {
        if (i != 0 && strcmp(args[i - 1], "&") == 0) {
          bgFlag = 1;
          args[i - 1] = NULL;
        } else {
          bgFlag = 0;
        }
        break;
      }
      args[i] = strdup(token);
      token = strtok(NULL, " ");
      ++i;
    }

    pid_t pid;
    pid = fork();
    if (pid < 0) {
      fprintf(stderr, "Fork Failed!\n");
      return EXIT_FAILURE;
    } else if (pid == 0) {
      int error = execvp(args[0], args);
      if (error == -1) {
        printf("Invalid Command!\n");
        return EXIT_FAILURE;
      }
    } else {
      if (!bgFlag) {
        wait(NULL);
      }
    }
  }
  return EXIT_SUCCESS;
}
