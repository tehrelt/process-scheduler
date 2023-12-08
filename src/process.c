#include "../include/process.h"

process_t *process_create(int id) {
  process_t *proccess = (process_t *)malloc(sizeof(process_t));
  proccess->pid = id;
  return proccess;
}
