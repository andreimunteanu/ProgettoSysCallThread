#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct operation{
  int num1;
  char op;
  int num2;
  float res;
}operation;

typedef operation* op_addr;

typedef struct thread_arg{
  pthread_mutex_t *my_mutex;
  pthread_mutex_t *global_mutex;
  pthread_cond_t *synchronizer;
  pthread_cond_t *father_sync;
  op_addr *address;
  int *remaining_work;
}thread_arg;

int read_integer(int fd);
void copy_operations(int fd, int *proc_id, operation **operations,int lines);
void *start(void *arg);
thread_arg *init_thread_args(int n_threads, int *lines);

