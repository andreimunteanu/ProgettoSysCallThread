#include "pcalc.h"

void *start(void *arg){/*
  thread_arg *my_arg = (thread_arg*)arg;
  pthread_mutex_t *my_mutex = my_arg->my_mutex;
  pthread_mutex_t *global_mutex = my_arg->global_mutex;
  pthread_cond_t *synchronizer = my_arg->synchronizer;
  pthread_cond_t *father_sync = my_arg->father_sync;
  op_addr *operation = my_arg->address;
  int *remaining_work = my_arg->remaining_work;
  char c;
  //printf("\t hello i'm child\n");
  sleep(2);
  // *operation;

  //if(*operation == NULL)
  printf("FIGLIO THREAD ARGS: my_arg = %p\tmy_mutex = %p\tremaining_work = %p\n********************************\n",my_arg , my_mutex, remaining_work);//,(**operation).num1, (**operation).op,(**operation).num2);
    /*
  while(0){
       //printf("\t %d my_operation %d %c %d\n",sem_num,my_operation.num1,my_operation.op, my_operation.num2);
    // printf("\t %c %d \n",(**address).op,(**address).num1);
    //printf("\tfiglio: %d,op add %p \n",sem_num,*address);       
    
    //printf("op %c ", my_operation.op);
    switch((**operation).op) {
    case '+': (**operation).res = (**operation).num1 + (**operation).num2; break;
    case '-': (**operation).res = (**operation).num1 - (**operation).num2; break;
    case '*': (**operation).res = (**operation).num1 * (**operation).num2; break;
    case '/': (**operation).res = (float)(**operation).num1 / (float)(**operation).num2; break;
    case 'K': printf("Muoro"); return;
    default : printf("No such operation!\n");
    }
    //printf("res %f\n",my_operation.res);
   
    //return;
    }*/
  pthread_exit(NULL);
}
