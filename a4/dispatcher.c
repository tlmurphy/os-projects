/**
 * Project 4
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "Queue.h"
#include "Process.h"


void terminateProcess(/* arguments */) {
  /* code */
}

void suspendProcess(/* arguments */) {
  /* code */
}

void startProcess() {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Failed!\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    int error = execvp("./process", NULL);
    if (error == -1) {
      puts("process command has failed, exiting");
      exit(EXIT_FAILURE); // Need to exit child if command is a failure
    }
  } else {
    waitpid(pid, NULL, 0);
  }
}

void restartProcess(/* arguments */) {
  /* code */
}

void readInput(char *fileName, Queue *q) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("sampleInput.txt", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    char *token;
    token = strtok(line, ",");
    int arrival = atoi(token);
    token = strtok(NULL, ",");
    int priority = atoi(token);
    token = strtok(NULL, ",");
    int ptime = atoi(token);
    token = strtok(NULL, ",");
    Process *p = newProcess(arrival, priority, ptime);
    enqueue(q, p);
    token = strtok(NULL, ",");
  }

  fclose(fp);
  if (line)
    free(line);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Please provide the input file as the argument!");
    exit(EXIT_FAILURE);
  }
  Queue *q;
  q = newQueue();
  readInput(argv[1], q);
  printQueue(q);
  startProcess();
  exit(EXIT_SUCCESS);
}
