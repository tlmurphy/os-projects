#ifndef PROCESS_H
#define PROCESS_H

typedef struct ProcessObject {
  int arrival;
  int priority;
  int ptime;
} Process;

extern Process *newProcess(int arrival, int priority, int ptime);

#endif /* PROCESS_H */
