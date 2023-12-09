#include "../include/rr.h"

process_t *rr_do_tick(queue_t *q, int remaining_quantum) {
  process_t *proc = (process_t *)dequeue(q);

  proc->status = PROC_RUNNING;

  --proc->remaining_time;

  if (proc->remaining_time == 0) {
    proc->status = PROC_COMPLETED;
  } else if (remaining_quantum == 0) {
    ++proc->priority;
    proc->status = PROC_WAITING;
  }

  return proc;
}
