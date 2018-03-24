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

int timer = 0; // Global timer for processes

void terminateProcess(Process *p) {
  int status;
  kill(p->pid, SIGINT);
  waitpid(p->pid, &status, WUNTRACED);
}

void suspendProcess(Process *p) {
  int status;
  kill(p->pid, SIGTSTP);
  waitpid(p->pid, &status, WUNTRACED);
}

void restartProcess(Process *p) {
  kill(p->pid, SIGCONT);
}

void startProcess(Process *p) {
  // Check if process hasn't started yet; if it has, just restart it
  if (p->pid == 0) {
    p->pid = fork();
    if (p->pid < 0) {
      fprintf(stderr, "Fork Failed!\n");
      exit(EXIT_FAILURE);
    } else if (p->pid == 0) {
      char *args[3] = {NULL};
      char ptime[50];
      sprintf(ptime, "%d", p->ptime);
      args[0] = "./process";
      args[1] = ptime;
      int error = execvp(args[0], args);
      if (error == -1) {
        puts("process command has failed, exiting");
        exit(EXIT_FAILURE); // Need to exit child if command is a failure
      }
    }
  } else {
    restartProcess(p);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Please provide the input file as the argument!");
    exit(EXIT_FAILURE);
  }
  /**
   * d  -> dispatcher input queue
   * s  -> system process queue
   * q1 -> priority 1 process queue
   * q2 -> priority 2 process queue
   * q3 -> priority 3 process queue
   */
  Queue *d = newQueue(), *s = newQueue(), *q1 = newQueue(), *q2 = newQueue(), *q3 = newQueue();
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  // Fill dispatch queue
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
    sortedEnqueue(d, p);
    token = strtok(NULL, ",");
  }
  fclose(fp);
  if (line)
    free(line);

  Process *currentProcess = NULL;
  Node *node = NULL;
  while (nonEmpty(d) == 0 || nonEmpty(s) == 0 || nonEmpty(q1) == 0 || nonEmpty(q2) == 0 || nonEmpty(q3) == 0 || currentProcess != NULL) {
    // Go through dispatch queue and add processes to their appropriate priority queue
    while (d->front != NULL && d->front->process->arrival <= timer) {
      node = dequeue(d);
      switch (node->process->priority) {
        case 0: enqueue(s, node->process); break;
        case 1: enqueue(q1, node->process); break;
        case 2: enqueue(q2, node->process); break;
        case 3: enqueue(q3, node->process); break;
      }
      node = NULL;
    }
    if (currentProcess != NULL) {
      currentProcess->ptime--;
      if (currentProcess->ptime <= 0) {
        terminateProcess(currentProcess);
        currentProcess = NULL;
      } else if (nonEmpty(s) == 0 || nonEmpty(q1) == 0 || nonEmpty(q2) == 0 || nonEmpty(q3) == 0) {
        suspendProcess(currentProcess);
        if (currentProcess->priority <= 3) {
          if (currentProcess->priority < 3) {
            currentProcess->priority++;
          }
          switch (currentProcess->priority) {
            case 2: enqueue(q2, currentProcess); break;
            case 3: enqueue(q3, currentProcess); break;
          }
        }
        currentProcess = NULL;
      } else {
        // Nothing in the rest of the queues, so just continue with current process
        suspendProcess(currentProcess);
        startProcess(currentProcess);
        sleep(1);
      }
    } else if (currentProcess == NULL) {
      Node *temp = NULL;
      // This if-else structure ensures we handle the highest priority processes first
      if (nonEmpty(s) == 0) {
        temp = dequeue(s);
        currentProcess = temp->process;
        startProcess(currentProcess);
        // Since system process, run until completion then set current process to none
        sleep(currentProcess->ptime);
        timer = timer + currentProcess->ptime;
        terminateProcess(currentProcess);
        currentProcess = NULL;
      } else if (nonEmpty(q1) == 0) {
        temp = dequeue(q1);
        currentProcess = temp->process;
        startProcess(currentProcess);
        sleep(1);
        timer++;
      } else if (nonEmpty(q2) == 0) {
        temp = dequeue(q2);
        currentProcess = temp->process;
        startProcess(currentProcess);
        sleep(1);
        timer++;
      } else if (nonEmpty(q3) == 0) {
        temp = dequeue(q3);
        currentProcess = temp->process;
        startProcess(currentProcess);
        sleep(1);
        timer++;
      } else {
        timer++;
      }
    }
  }
  exit(EXIT_SUCCESS);
}
