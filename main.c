#include "include/process.h"
#include "include/queue.h"
#include "include/scheduler.h"
#include "include/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand((unsigned)ts.tv_nsec);

  scheduler_t *scheduler = init_scheduler();

  queue_t *q = scheduler->processes_q;

  generate_processes(scheduler, 20);

  printf("Total burst: %d\n", total_burst(scheduler));
}
