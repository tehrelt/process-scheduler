#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"

#define SCH_Q_FCFS 0
#define SCH_Q_RR 1

typedef struct scheduler {
  int quantum;
  queue_t *processes_q;
  queue_t *queues_q;

} scheduler_t;

typedef struct sch_queue {
  queue_t *queue;
  int type;
} sch_queue_t;

scheduler_t *init_scheduler(int quantum);
int total_burst(scheduler_t *s);
void generate_processes(scheduler_t *s, int count, int max_burst);

void rr_do_tick(process_t *proc, int remaining_quantum);
void fcfs_do_tick(process_t *proc);

#endif // !SCHEDULER_H
