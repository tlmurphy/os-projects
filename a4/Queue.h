#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"

typedef struct QueueObject {
  Node *front;
  Node *back;
} Queue;

extern Queue *newQueue();
extern void enqueue(Queue *q,  Process *p);
extern void sortedEnqueue(Queue *q, Process *p);
extern Node *dequeue(Queue *q);
extern void printQueue(Queue *q);
extern int nonEmpty(Queue *q);

#endif /* QUEUE_H */
