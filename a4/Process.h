#ifndef PROCESS_H
#define PROCESS_H

typedef struct ProcessObject {
  int arrival;
  int priority;
  int ptime;
  int pid;
} Process;

extern Process *newProcess(int arrival, int priority, int ptime);

#endif /* PROCESS_H */
