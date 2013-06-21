#include "pcalc.h"

void *start(void *arg){
  thread_arg *my_arg = (thread_arg*)arg;
  pthread_mutex_t *my_mutex = my_arg->my_mutex;
  pthread_mutex_t *global_mutex = my_arg->global_mutex;
  pthread_cond_t *synchronizer = my_arg->synchronizer;
  pthread_cond_t *father_sync = my_arg->father_sync;
  pthread_cond_t *father_hold = my_arg->father_hold;
  operation *operations = my_arg->operations;
  operation *my_op;
  int *offset = my_arg->offset;
  int *remaining_work = my_arg->remaining_work;
  int *available_workers = my_arg->available_workers;
  
  //printf("\t hello i'm child\n");
 
  
  while(1){
    pthread_mutex_lock(my_mutex);
    if(*offset == -1)
      pthread_cond_wait(synchronizer, my_mutex);
    
    pthread_mutex_lock(global_mutex);
    --(*available_workers);
    pthread_mutex_unlock(global_mutex);
    my_op = operations + *offset;
    printf("\tmy_operation %d %c %d\n", my_op->num1,my_op->op, my_op->num2);    // printf("\t %c %d \n",(**address).op,(**address).num1);

    //printf("\tfiglio: %d,op add %p \n",sem_num,*address);       
    
    //printf("op %c ", my_operation.op)
    switch(my_op->op) {
    case '+': my_op->res = my_op->num1 + my_op->num2; break;
    case '-': my_op->res = my_op->num1 - my_op->num2; break;
    case '*': my_op->res = my_op->num1 * my_op->num2; break;
    case '/': my_op->res = (float)my_op->num1 / (float)my_op->num2; break;
    case 'K': pthread_exit(NULL);
    default : printf("No such operation!\n");
    }
    
    *offset = -1;
    pthread_cond_signal(synchronizer);
    pthread_mutex_lock(global_mutex);
    if((*available_workers)++ == 0)
      pthread_cond_signal(father_hold);
    if(--(*remaining_work) == 0)
      pthread_cond_signal(father_sync);
    pthread_mutex_unlock(global_mutex);
    pthread_mutex_unlock(my_mutex);
  }
  
}
