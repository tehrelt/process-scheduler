#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#define PROC_READY 0
#define PROC_RUNNING 1
#define PROC_WAITING 2
#define PROC_COMPLETED 3

typedef struct process {
  int pid;
  int burst;
  int remaining_time;
  int priority;
  int status;
} process_t;

process_t *process_create(int id);

#endif // !PROCESS_H
