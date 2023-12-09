#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
typedef struct scheduler {
  int quantum;
  queue_t *processes_q;
  queue_t *queues_q;

} scheduler_t;

scheduler_t *init_scheduler(int quantum);
int total_burst(scheduler_t *s);
void generate_processes(scheduler_t *s, int count, int max_burst);

#endif // !SCHEDULER_H
