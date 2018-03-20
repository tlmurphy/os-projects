#ifndef NODE_H
#define NODE_H

#include "Process.h"

typedef struct NodeObject {
  Process *process;
  struct NodeObject *next;
} Node;

extern Node *newNode(Process *p);

#endif /* NODE_H */
