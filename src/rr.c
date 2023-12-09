#include "../include/rr.h"

void rr_do_tick(process_t *proc, int remaining_quantum) {
  proc->status = PROC_RUNNING;

  --proc->remaining_time;

  if (proc->remaining_time <= 0) {
    proc->status = PROC_COMPLETED;
  } else if (remaining_quantum == 0) {
    ++proc->priority;
    proc->status = PROC_WAITING;
  }
}

void fcfs_do_tick(process_t *proc) {
  proc->status = PROC_RUNNING;

  --proc->remaining_time;

  if (proc->remaining_time <= 0) {
    proc->status = PROC_COMPLETED;
  }
}
