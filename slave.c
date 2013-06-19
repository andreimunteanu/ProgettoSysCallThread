#include "pcalc.h"

void *start(void *arg){
   thread_arg *my_arg = (thread_arg*)arg;
   pthread_mutex_t *my_mutex = my_arg->my_mutex;
   // pthread_mutex_t *global_mutex = my_arg->global_mutex;
  pthread_cond_t *synchronizer = my_arg->synchronizer;
  // pthread_cond_t *father_sync = my_arg->father_sync;
  // op_addr *operation = my_arg->address;
  // int *remaining_work = my_arg->remaining_work;
    // char *c=(char*)arg;
 printf("\t hello i'm child\n");
 // sleep(5);
  // *operation;

  //if(*operation == NULL)
 printf("\t hello again syn init at true %p %p \n",my_arg,my_mutex);
 //printf("\t hello again syn init at true %p \n",c);
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
 sleep(1);
 pthread_exit(NULL);
 //exit(0);
}
