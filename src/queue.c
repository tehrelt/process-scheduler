#include "../include/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

queue_t *init_queue() {
  // printf("Allocating memory for queue\n");
  queue_t *q = (queue_t *)malloc(sizeof(queue_t));

  q->head = NULL;
  q->tail = NULL;
  q->count = 0;

  return q;
}

void enqueue(queue_t *queue, void **item) {
  node_t *new_node = (node_t *)malloc(sizeof(node_t));

  new_node->item = *item;
  new_node->next = NULL;

  if (queue->head == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
    ++queue->count;
    return;
  }

  queue->tail->next = new_node;
  queue->tail = new_node;

  ++queue->count;
}

void *dequeue(queue_t *queue) {
  if (queue->head == NULL) {
    return NULL;
  }

  void *item = queue->head->item;
  node_t *temp = queue->head;
  queue->head = queue->head->next;
  free(temp);

  --queue->count;

  return item;
}

void *peek(queue_t *queue, int index) {

  if (index < 0 && index >= queue->count) {
    return NULL;
  }

  node_t *p = queue->head;

  for (int i = 0; i < index; i++) {
    p = p->next;
  }

  return p->item;
}
