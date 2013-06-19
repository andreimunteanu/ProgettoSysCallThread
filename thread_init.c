#include"pcalc.h"

pthread_cond_t *init_child_cond(){
   pthread_cond_t *child_sync = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
  
     if(pthread_cond_init(child_sync,NULL)){
       perror("child mutex and cond");
       exit(1);
     }
  
   return child_sync;
}

pthread_mutex_t *init_mutex(){
  pthread_mutex_t *child_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    if(pthread_mutex_init(child_mutex,NULL)){
      perror("child mutex");
      exit(1);
    }

  return child_mutex;
}
thread_arg *create_thread_arg(){
  thread_arg *arg = (thread_arg*)malloc(sizeof(thread_arg));
  arg->my_mutex = init_mutex();
  arg->synchronizer = init_child_cond();
  return arg;
} 

thread_arg **init_thread_args(int n_threads,int* lines){
  
  thread_arg **thread_args = (thread_arg**)malloc(n_threads*sizeof(thread_arg*));
  // pthread_mutex_t *child_mutex;
  //pthread_mutex_t *global_mutex;
  //pthread_cond_t *child_sync = (pthread_cond_t*)malloc(n_threads*sizeof(pthread_cond_t));
  //pthread_cond_t *father_sync = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
  //op_addr *operations = (op_addr*)malloc(n_threads*sizeof(op_addr));

  int i;
  thread_arg **temp = thread_args;
  /*
  if(pthread_mutex_init(global_mutex,NULL)){
    perror("global mutex");
    exit(1);
  }
 
  if(pthread_cond_init(father_sync,NULL)){
    perror("father sync");
    exit(1);
    }*/
  /*
  for(i = 0; i < n_threads; ++i){
    temp = thread_args + i*sizeof(thread_arg);
    temp->my_mutex = child_mutex+i*sizeof(pthread_mutex_t);
    temp->global_mutex = global_mutex;
    temp->synchronizer = child_sync + i*sizeof(pthread_cond_t);
    temp->father_sync = father_sync;
    temp->address = operations +i*sizeof(op_addr);
    temp->remaining_work = lines;
    printf("Init thrargs %p\n", temp);
  }
  */
  for(i = 0; i < n_threads; ++i){
    *(temp + i*sizeof(thread_arg*)) = create_thread_arg();
  }
    
  
  return thread_args;
}
