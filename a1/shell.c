#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LINE 80 // The maximum length command
#define MAX_SIZE MAX_LINE / 2 + 1

int main(int argc, char const *argv[]) {
  char input[MAX_SIZE];
  char * args[MAX_SIZE]; // command line arguments
  char * commandHistory[1000] = {}; // Surely 1000 commands is enough...
  char * token;

  int bgFlag = 0;
  int shouldRun = 1; // flag to determine when to exit program
  int commandCount = 0;
  while (shouldRun) {
    memset(args, 0, sizeof(args)); // Clear input before retrieving it again

    printf("osh> ");

    fflush(stdout);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Strip newline from input since it is a pain

    if (!strcmp(input, "")) {
      // Just move on to the next prompt if no input
      continue;
    }

    if (!strcmp(input, "history")) {
      for (int i = 0; i < commandCount; i++) {
        printf("%d %s\n", commandCount - i, commandHistory[i]);
      }
      continue;
    }

    if (input[0] == '!') {
      if (!strcmp(input, "!!")) {
        if (commandCount == 0) {
          puts("No commands in history.");
          continue;
        }
        strcpy(input, commandHistory[commandCount - 1]);
      } else {
        char nCommand[20] = "";
        int badInput = 0;
        for (size_t i = 1; input[i] != '\0'; i++) {
          if (!isdigit(input[i])) {
            // User tries something like !ab3k
            badInput = 1;
            break;
          }
          nCommand[i - 1] = input[i];
        }
        if (!badInput) {
          int n = atoi(nCommand);
          if (n > 1000 || n == 0 || commandHistory[n - 1] == NULL) {
            printf("No such command in history.\n");
            continue;
          }
          strcpy(input, commandHistory[commandCount - n]);
        }
      }
    }

    if (!strcmp(input, "exit")) {
      shouldRun = 0;
      continue;
    }

    commandHistory[commandCount] = strdup(input);
    commandCount += 1;

    token = strtok(input, " ");
    int i = 0;
    while (1) {
      if (token == NULL) {
        if (i != 0 && !strcmp(args[i - 1], "&")) {
          bgFlag = 1;
          args[i - 1] = NULL; // Remove '&' since we don't need it for the command
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
      execvp(args[0], args);
      int error = execvp(args[0], args);
      if (error == -1) {
        printf("Invalid Command!\n");
        return EXIT_FAILURE; // Need to exit child if command is a failure
      }
    } else {
      if (!bgFlag) {
        wait(NULL);
      }
    }
  }
  return EXIT_SUCCESS;
}
