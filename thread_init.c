/**
 * @file   thread_init.c
 * @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
 * @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
 * @date   July, 2013
 * @brief  Thread initialization
 * 
 */
#include"header.h"

thread_arg *init_thread_args(int n_threads,int* lines, operation *operations, int *available_workers){
  int i;
  thread_arg *temp;
  thread_arg *thread_args = (thread_arg*)malloc(n_threads*sizeof(thread_arg));
  pthread_mutex_t *child_mutex = (pthread_mutex_t*)malloc(n_threads*sizeof(pthread_mutex_t));
  pthread_mutex_t *global_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_cond_t *child_sync = (pthread_cond_t*)malloc(n_threads*sizeof(pthread_cond_t));
  pthread_cond_t *father_sync = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
  pthread_cond_t *father_hold = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
  int *offsets = (int *)malloc(n_threads*sizeof(int));
  int *status = (int*)malloc(n_threads*sizeof(int));
    
   for(i = 0; i < n_threads; ++i){
     if(pthread_mutex_init(child_mutex +i,NULL) ||
	pthread_cond_init(child_sync + i, NULL)){
      perror("child mutex and cond");
      exit(1);
    }
    }

  if(pthread_mutex_init(global_mutex,NULL)){
    perror("global mutex");
    exit(1);
  }
 
  if(pthread_cond_init(father_sync,NULL) || pthread_cond_init(father_hold,NULL)){
    perror("father sync");
    exit(1);
  }
  
  for(i = 0; i < n_threads; ++i){
    temp = thread_args + i;
    temp->my_mutex = child_mutex+i;
    temp->global_mutex = global_mutex;
    temp->synchronizer = child_sync + i;
    temp->father_sync = father_sync;
    temp->father_hold = father_hold;
    temp->operations = operations;
    temp->thread_id = i + 1;
    temp->status = status +i;
    *temp->status = -2;// Never worked.
    temp->offset = offsets +i;
    temp->remaining_work = lines;
    temp->available_workers = available_workers;
  }
  
  return thread_args;
}
