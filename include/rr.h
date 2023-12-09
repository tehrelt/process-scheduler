#include "process.h"
#include "queue.h"

void rr_do_tick(process_t *proc, int remaining_quantum);
void fcfs_do_tick(process_t *proc);
