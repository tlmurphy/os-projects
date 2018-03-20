// Node struct for Queue
// Contains a Process as its data

#include <stdio.h>
#include <stdlib.h>
#include "Node.h"

Node *newNode(Process *p) {
  Node *n = malloc(sizeof(Node));
  if (n == 0) { fprintf(stderr, "NOT ENOUGH MEMORY...EXITING"); exit(-1); }
  n->process = p;
  n->next = NULL;
  return n;
}
