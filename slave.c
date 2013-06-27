/**
 * @file   slave.c
 * @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
 * @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
 * @date   July, 2013
 * @brief  contains the main function for the threads
 * 
 */
#include "header.h"

void *start(void *arg){
  thread_arg *my_arg = (thread_arg*)arg;
  pthread_mutex_t *my_mutex = my_arg->my_mutex;
  pthread_mutex_t *global_mutex = my_arg->global_mutex;
  pthread_cond_t *synchronizer = my_arg->synchronizer;
  pthread_cond_t *father_sync = my_arg->father_sync;
  pthread_cond_t *father_hold = my_arg->father_hold;
  operation *operations = my_arg->operations;
  operation *my_op;
  int id = my_arg->thread_id;
  int *status = my_arg->status;
  int *offset = my_arg->offset;
  int *remaining_work = my_arg->remaining_work;
  int *available_workers = my_arg->available_workers;
  char buf[64];
  
  while(1){
    pthread_mutex_lock(my_mutex);
    if(*status < 0) //Operation not assigned.
      pthread_cond_wait(synchronizer, my_mutex);
    
    pthread_mutex_lock(global_mutex);
    --(*available_workers);
    pthread_mutex_unlock(global_mutex);
    my_op = operations + *offset; //Takes the operation.
    sprintf(buf, "\t[THREAD %d] operation ==> %d %c %d\n",id, my_op->num1,my_op->op, my_op->num2);
    print_to_video(buf);

    switch(my_op->op) {
    case '+': my_op->res = my_op->num1 + my_op->num2; break;
    case '-': my_op->res = my_op->num1 - my_op->num2; break;
    case '*': my_op->res = my_op->num1 * my_op->num2; break;
    case '/': my_op->res = (float)my_op->num1 / (float)my_op->num2; break;
    case 'K': pthread_exit(NULL);
    default : sprintf(buf,"No such operation!\n");
      print_to_video(buf);
    }
    
    *status = -1;//Child available.
    pthread_cond_signal(synchronizer);//Wakes up the father.
    pthread_mutex_lock(global_mutex);
    if((*available_workers)++ == 0)//Wakes up the father if a worker is available.
      pthread_cond_signal(father_hold);
    if(--(*remaining_work) == 0)
      pthread_cond_signal(father_sync);//Wakes up the father if all operations are done.
    pthread_mutex_unlock(global_mutex);
    pthread_mutex_unlock(my_mutex);
  }  
}
