#include "include/process.h"
#include "include/queue.h"
#include <stdio.h>
#include <stdlib.h>

void print_queue(queue_t *q) {
  node_t *p = q->head;

  while (p != NULL) {
    printf("%d->", ((process_t *)(p->item))->pid);
    p = p->next;
  }
  printf("NULL\n");
}

int main(int argc, char **argv) {
  // if (argc < 3) {
  //   printf("Usage: ./main <quantum> <burst>");
  //   exit(EXIT_FAILURE);
  // }

  queue_t *q = init_queue();

  for (int i = 0; i < 5; ++i) {
    process_t *proc = process_create(i);
    enqueue(q, (void **)&proc);
    print_queue(q);
  }

  process_t *proc;
  proc = (process_t *)dequeue(q);
  printf("dequeued pid: %d\n", proc->pid);
  print_queue(q);
  proc = (process_t *)dequeue(q);
  printf("dequeued pid: %d\n", proc->pid);
  print_queue(q);

  proc = (process_t *)dequeue(q);
  printf("dequeued pid: %d\n", proc->pid);
  print_queue(q);

  proc = (process_t *)dequeue(q);
  printf("dequeued pid: %d\n", proc->pid);
  print_queue(q);
}
