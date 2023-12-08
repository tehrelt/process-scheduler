#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"

typedef struct node {
  process_t item;
  struct node *next;
} node_t;

typedef struct queue {
  node_t *head;
  node_t *tail;
  int count;
} queue_t;

queue_t *init_queue();
void enqueue(queue_t *queue, process_t *item);
void dequeue(queue_t *queue);

#endif // !QUEUE_H
