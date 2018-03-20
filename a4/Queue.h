#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"

typedef struct QueueObject {
  Node *front;
  Node *back;
} Queue;

extern Queue *newQueue();
extern void enqueue(Queue *q,  Process *p);
extern void dequeue(Queue *q);
extern void printQueue(Queue *q);

#endif /* QUEUE_H */
