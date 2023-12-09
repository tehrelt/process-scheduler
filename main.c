#include "include/process.h"
#include "include/queue.h"
#include "include/rr.h"
#include "include/scheduler.h"
#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

FILE *fd = NULL;

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

  if ((fd = fopen(".log", "w+")) == NULL) {
    perror("open fd");
    exit(EXIT_FAILURE);
  }
}

void run(scheduler_t *scheduler);
void open_viewer(scheduler_t *scheduler);

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
    printf("4. Run\n");
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
      fseek(fd, 0, 0);
      fwrite(&scheduler->processes_q->count, sizeof(int), 1, fd);
      break;

    case 4:
      run(scheduler);
      break;
    case 5:
      open_viewer(scheduler);
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

  fclose(fd);
}

int queues_zapolnenost(scheduler_t *s) {
  int total = 0;

  node_t *current_queue = s->queues_q->head;
  while (current_queue != NULL) {
    total += ((queue_t *)current_queue->item)->count;
    current_queue = current_queue->next;
  }

  return total;
}

int pow(int x, int y) {
  if (y == 1)
    return x;
  return x * pow(x, y - 1);
}

void write_to_file(scheduler_t *scheduler, int tick) {

  int tick_size = scheduler->processes_q->count * sizeof(process_t);

  int offset = sizeof(int) + (tick * tick_size);
  // printf("offset: %d\n", offset);
  fseek(fd, offset, 0);
  for (node_t *node = scheduler->processes_q->head; node != NULL;
       node = node->next) {
    fwrite(node->item, sizeof(process_t), 1, fd);
  }
}

void print_tick(int tick) {
  int count = 0;
  process_t proc;

  fseek(fd, 0, 0);
  fread(&count, sizeof(int), 1, fd);
  int tick_size = count * sizeof(process_t);
  printf("count: %d\ntick_size: %d\ntick_offset: %d\n", count, tick_size,
         tick_size * tick);

  fseek(fd, sizeof(int) + (tick_size * tick), 0);

  printf("pid\tpri\tburst\trt\tstatus\n");

  for (int i = 0; i < count; i++) {
    fread(&proc, sizeof(process_t), 1, fd);
    printf("%d\t%d\t%d\t%d\t%s\n", proc.pid, proc.priority, proc.burst,
           proc.remaining_time,
           proc.status == PROC_RUNNING     ? "R"
           : proc.status == PROC_WAITING   ? "W"
           : proc.status == PROC_COMPLETED ? "C"
                                           : "RD");
  }
}

void run(scheduler_t *scheduler) {
  int queue_count = 3;
  for (int i = 0; i < queue_count; ++i) {
    printf("enqueue a queue\n");
    queue_t *q = init_queue();
    enqueue(scheduler->queues_q, (void **)&q);
  }
  printf("\n");

  queue_t *q = (queue_t *)(node_t *)(scheduler->queues_q->head)->item;
  for (node_t *node = scheduler->processes_q->head; node != NULL;
       node = node->next) {
    printf("enqueue a proc(%d) to queue\n", ((process_t *)node->item)->pid);
    process_t *proc = (process_t *)node->item;
    enqueue(q, (void **)&proc);
  }
  printf("\n");

  int tick = 0;
  write_to_file(scheduler, tick++);
  int queue_i = 0;
  while (scheduler->queues_q->count != 0) {
    queue_t *q = dequeue(scheduler->queues_q);
    printf("\ndequeued queue with %d elements head: %d\n", q->count,
           ((process_t *)q->head->item)->pid);
    for (int queue_quantum = scheduler->quantum * (++queue_i % queue_count);
         queue_quantum != 0; --queue_quantum) {
      process_t *proc = (process_t *)dequeue(q);
      printf("dequeued process: %d\n", proc->pid);
      int current_quantum = scheduler->quantum;
      while (current_quantum != 0) {
        if (proc->priority == 2) {
          printf("executing fcfs on pid=%d\t", proc->pid);
          fcfs_do_tick(proc);
          printf("executed fcfs rt=%d\n", proc->remaining_time);
          if (proc->status == PROC_COMPLETED) {
            current_quantum = 0;
          }
        } else {
          printf("executing rr on q=%d pid=%d\t", current_quantum, proc->pid);
          rr_do_tick(proc, --current_quantum);
          printf("rr executed pri = %d\n", proc->priority);
          if (proc->status == PROC_COMPLETED) {
            current_quantum = 0;
          }
        }
        write_to_file(scheduler, tick++);
        printf("logged\n");
      }

      if (proc->status != PROC_COMPLETED) {
        enqueue(scheduler->queues_q->head->item, (void **)&proc);
        printf("proc(%d) enqueued to next queue\n", proc->pid);
      }

      if (q->count == 0) {
        break;
      }
    }
    if (q->count == 0) {
      printf("queue is empty\n");
      free(q);
      break;
    } else {
      printf("enqeue q\n");
      enqueue(scheduler->queues_q, (void **)&q);
      printf("enqueued q\n");
    }
  }
}

void open_viewer(scheduler_t *scheduler) {
  int tick = 0;
  int view = 1;
  do {
    system("clear");
    print_tick(tick);
    printf("(b) - back\t(n) - next\t(c) - close current tick: %d\n", tick);
    char ch;
    do {
      ch = fgetc(stdin);
    } while (ch != 'b' && ch != 'n' && ch != 'c');

    if (ch == 'n') {
      ++tick;
    } else if (ch == 'b' && tick > 0) {
      --tick;
    } else if (ch == 'c') {
      view = 0;
    }
  } while (view == 1);
}
