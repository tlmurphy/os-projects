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

int timer = 0;

void terminateProcess(Queue *q, Process *p) {
  dequeue(q);
  kill(p->pid, SIGINT);
}

void suspendProcess(Queue *q, Queue *qLower, Process *p) {
  if (p->priority < 3)
    p->priority++;
  dequeue(q);
  enqueue(qLower, p);
  kill(p->pid, SIGTSTP);
  int status;
  waitpid(p->pid, &status, WUNTRACED);
}

void restartProcess(Process *p) {
  kill(p->pid, SIGINT);
}

void startProcess(Queue *q, Queue *qLower, Process *p) {
  if (p->pid != 0) {
    if (p->ptime == 0) {
      terminateProcess(q, p);
    } else {
      restartProcess(p);
    }
    printf("time left: %d\n", p->ptime);
    p->ptime--;
    return;
  }
  pid_t pid;
  pid = fork();
  p->pid = pid;
  if (pid < 0) {
    fprintf(stderr, "Fork Failed!\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    char *args[3] = {NULL};
    char ptime[50];
    sprintf(ptime, "%d", p->ptime);
    printf("p->time: %d\n", p->ptime);
    args[0] = "./process";
    args[1] = ptime;
    int error = execvp(args[0], args);
    if (error == -1) {
      puts("process command has failed, exiting");
      exit(EXIT_FAILURE); // Need to exit child if command is a failure
    }
  } else {
    if (p->priority == 0) {
      sleep(p->ptime);
      terminateProcess(q, p);
    } else {
      sleep(1);
      if (p->ptime == 0) {
        terminateProcess(q, p);
      } else {
        suspendProcess(q, qLower, p);
      }
    }
    p->ptime--;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Please provide the input file as the argument!");
    exit(EXIT_FAILURE);
  }
  Queue *d = newQueue(), *s = newQueue(), *q1 = newQueue(), *q2 = newQueue(), *q3 = newQueue();
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("sampleInput.txt", "r");
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
    enqueue(d, p);
    token = strtok(NULL, ",");
  }
  fclose(fp);
  if (line)
    free(line);

  // Go through dispatch queue
  Node *iter = d->front;
	while (iter != NULL) {
    Process *p = iter->process;
		if (p->arrival == timer) {
      switch (p->priority) {
        case 0: enqueue(s, p); break;
        case 1: enqueue(q1, p); break;
        case 2: enqueue(q2, p); break;
        case 3: enqueue(q3, p); break;
      }
      iter = iter->next;
    } else {
      // Go through all the queues and run the processes in them
      Node *sIter = s->front;
      Node *q1Iter = q1->front;
      Node *q2Iter = q2->front;
      Node *q3Iter = q3->front;
      while (sIter != NULL) {
        startProcess(s, s, sIter->process);
        sIter = sIter->next;
      }
      while (q1Iter != NULL) {
        startProcess(q1, q2, q1Iter->process);
        q1Iter = q1Iter->next;
      }
      while (q2Iter != NULL) {
        startProcess(q2, q3, q2Iter->process);
        q2Iter = q2Iter->next;
      }
      while (q3Iter != NULL) {
        startProcess(q3, q3, q3Iter->process);
        q3Iter = q3Iter->next;
      }
      timer++;
    }
	}

  // Go through all the queues and run the processes in them again...
  Node *sIter = s->front;
  Node *q1Iter = q1->front;
  Node *q2Iter = q2->front;
  Node *q3Iter = q3->front;
  while(sIter != NULL || q1Iter != NULL || q2Iter != NULL || q3Iter != NULL) {
    while (sIter != NULL) {
      startProcess(s, s, sIter->process);
      sIter = sIter->next;
    }
    while (q1Iter != NULL) {
      startProcess(q1, q2, q1Iter->process);
      q1Iter = q1Iter->next;
    }
    while (q2Iter != NULL) {
      startProcess(q2, q3, q2Iter->process);
      q2Iter = q2Iter->next;
    }
    while (q3Iter != NULL) {
      startProcess(q3, q3, q3Iter->process);
      q3Iter = q3Iter->next;
    }
    // Reset these pointers since a process could be enqueued onto these queues.
    if (q2Iter == NULL || q3Iter == NULL) {
      q2Iter = q2->front;
      q3Iter = q3->front;
    }
  }

  exit(EXIT_SUCCESS);
}
