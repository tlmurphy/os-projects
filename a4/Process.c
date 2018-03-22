#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

Process *newProcess(int arrival, int priority, int ptime) {
  Process *p = malloc(sizeof(Process));
  if (p == 0) { fprintf(stderr, "NOT ENOUGH MEMORY...EXITING"); exit(-1); }
  p->arrival = arrival;
  p->priority = priority;
  p->ptime = ptime;
  p->pid = 0;
  return p;
}
