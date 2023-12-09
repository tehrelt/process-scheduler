#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/tools.h"
#include <stdlib.h>

int processes_count = 0;

scheduler_t *init_scheduler(int quantum) {
  // printf("Allocating memory for scheduler_t\n");
  scheduler_t *sch = (scheduler_t *)malloc(sizeof(scheduler_t));
  sch->quantum = quantum;
  // printf("Initializing queue in scheduler\n");
  sch->queues_q = init_queue();
  sch->processes_q = init_queue();

  return sch;
}

int total_burst(scheduler_t *s) {
  node_t *p = s->processes_q->head;
  int total = 0;

  while (p != 0) {
    total += ((process_t *)p->item)->burst;
    p = p->next;
  }

  return total;
}

void generate_processes(scheduler_t *s, int count, int max_burst) {
  for (int i = 0; i < count; ++i) {
    process_t *proc = process_create(processes_count++);
    proc->burst = rand_int(1, max_burst);
    proc->remaining_time = proc->burst;
    proc->priority = 0;
    proc->status = PROC_READY;

    enqueue(s->processes_q, (void **)&proc);
  }
}

void rr_do_tick(process_t *proc, int remaining_quantum) {
  proc->status = PROC_RUNNING;

  --proc->remaining_time;

  if (proc->remaining_time <= 0) {
    proc->status = PROC_COMPLETED;
  } else if (remaining_quantum == 0) {
    ++proc->priority;
    proc->status = PROC_READY;
  }
}

void fcfs_do_tick(process_t *proc) {
  proc->status = PROC_RUNNING;

  --proc->remaining_time;

  if (proc->remaining_time <= 0) {
    proc->status = PROC_COMPLETED;
  }
}
