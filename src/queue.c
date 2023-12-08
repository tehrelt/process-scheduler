#include "../include/queue.h"
#include "../include/process.h"
#include <stdlib.h>

queue_t *init_queue() {
  queue_t *q = (queue_t *)malloc(sizeof(queue_t));

  q->head = NULL;
  q->tail = NULL;
  q->count = 0;

  return q;
}

void enqueue(queue_t *queue, process_t *item) {
  node_t *new_node = (node_t *)malloc(sizeof(node_t));

  new_node->item = *item;
  new_node->next = NULL;

  if (queue->head == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
    return;
  }

  queue->tail->next = new_node;
  queue->tail = new_node;

  ++queue->count;
}
