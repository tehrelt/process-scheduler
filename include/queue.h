#ifndef QUEUE_H
#define QUEUE_H

typedef struct node {
  void **item;
  struct node *next;
} node_t;

typedef struct queue {
  node_t *head;
  node_t *tail;
  int count;
} queue_t;

queue_t *init_queue();
void enqueue(queue_t *queue, void **item);
void *dequeue(queue_t *queue);

#endif // !QUEUE_H
