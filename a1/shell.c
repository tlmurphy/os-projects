#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LINE 80 // The maximum length command
#define MAX_SIZE MAX_LINE / 2 + 1

void getInput(char *input) {
  fflush(stdout);
  fgets(input, MAX_SIZE, stdin);
  input[strcspn(input, "\n")] = '\0'; // Strip newline from input since it is a pain
}

int processCommand(char *input, char **commandHistory, int *commandCount, int *shouldRun) {
  if (!strcmp(input, "")) {
    // Just move on to the next prompt if no input
    return 1;
  }

  if (!strcmp(input, "history")) {
    for (int i = 0; i < *commandCount; i++) {
      printf("%d %s\n", *commandCount - i, commandHistory[i]);
    }
    return 1;
  }

  if (input[0] == '!') {
    if (!strcmp(input, "!!")) {
      if (*commandCount == 0) {
        puts("No commands in history.");
        return 1;
      }
      strcpy(input, commandHistory[*commandCount - 1]);
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
          puts("No such command in history.");
          return 1;
        }
        strcpy(input, commandHistory[*commandCount - n]);
      }
    }
  }

  if (!strcmp(input, "exit")) {
    *shouldRun = 0;
    return 1;
  }

  commandHistory[*commandCount] = strdup(input);
  *commandCount += 1;
  return 0;
}

void buildArguments(char *input, char **args, int *bgFlag) {
  char *token = strtok(input, " ");
  int i = 0;
  while (1) {
    if (token == NULL) {
      if (i != 0 && !strcmp(args[i - 1], "&")) {
        *bgFlag = 1;
        args[i - 1] = NULL; // Remove '&' since we don't need it for the command
      } else {
        *bgFlag = 0;
      }
      break;
    }
    args[i] = strdup(token);
    token = strtok(NULL, " ");
    ++i;
  }
}

void executeProcess(char **args, int bgFlag) {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Failed!\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    execvp(args[0], args);
    int error = execvp(args[0], args);
    if (error == -1) {
      puts("Invalid Command!");
      exit(EXIT_FAILURE); // Need to exit child if command is a failure
    }
  } else {
    if (!bgFlag) {
      waitpid(pid, NULL, 0);
    }
  }
}

int main(int argc, char const *argv[]) {
  char * commandHistory[1000] = {}; // Surely 1000 commands is enough...
  int bgFlag = 0;
  int shouldRun = 1; // flag to determine when to exit program
  int commandCount = 0;
  while (shouldRun) {
    char input[MAX_SIZE] = {};
    char * args[MAX_SIZE] = {}; // command line arguments
    printf("osh> ");
    getInput(input);
    if (processCommand(input, commandHistory, &commandCount, &shouldRun) == 1) {
      // Skip to next prompt
      continue;
    }
    buildArguments(input, args, &bgFlag);
    executeProcess(args, bgFlag);
  }
  return EXIT_SUCCESS;
}
