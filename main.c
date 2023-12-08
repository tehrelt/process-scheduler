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

void init() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand((unsigned)ts.tv_nsec);
}

int main(int argc, char **argv) {
  // if (argc < 3) {
  //   printf("Usage: ./main <quantum> <burst>");
  //   exit(EXIT_FAILURE);
  // }
  init();

  int choice = -1;

  scheduler_t *scheduler = init_scheduler(2);

  while (1) {
    fflush(stdin);
    printf("0. Exit\n");
    printf("1. Scheduler info\n");
    printf("2. Set quantum value\n");
    printf("3. Generate processes\n");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      printf("Schedulers info: \n");
      printf("quantum: %d\n", scheduler->quantum);
      printf("procs count: %d\n", scheduler->processes_q->count);
      printf("queues count: %d\n", scheduler->queues_q->count);
      printf("pid\tburst\n");

      for (node_t *node = scheduler->processes_q->head; node != NULL;
           node = node->next) {
        process_t *proc = (process_t *)node->item;
        printf("%d\t%d\n", proc->pid, proc->burst);
      }
      printf("Total burst: %d\n", total_burst(scheduler));
      break;

    case 2:
      printf("New quantum value: ");
      scanf("%d", &scheduler->quantum);
      break;

    case 3:
      printf("Generating processes\n");
      int count = 0, max_burst = 0;

      printf("Count of prcs: ");
      scanf("%d", &count);

      printf("Max burst: ");
      scanf("%d", &max_burst);
      generate_processes(scheduler, count, max_burst);
      break;

    case 0:
      printf("Exiting...\n");
      exit(0);
      break;

    default:
      printf("Invalid option...\n");
    }
    printf("\n\n");
  }
}
