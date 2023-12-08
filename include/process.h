#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#define READY 0
#define RUNNING 1
#define WAITING 2
#define COMPLETED 3

typedef struct process {
  int pid;
  int burst;
  int remaining_time;
  int priority;
  int status;
} process_t;

process_t *process_create(int id);

#endif // !PROCESS_H
