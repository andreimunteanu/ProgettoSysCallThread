#include "header.h"

void main(int argc, char *argv[]){
  int fd,n_threads,j,i = 0, lines = -1;  // da dichiarare i register
  int available_workers, remaining_work;
  int id;
  int *thread_ids;
  float *results;
  operation *operations;
  pthread_t *threads;
  thread_arg *thread_args;
  char *conf_file;
  char *filename;
  char buf[64];
  
  if(argc > 2){
    exit(1);
  }
  else{
    if(argc == 1)
      conf_file = prompt_user("insert file name: ");
    
    
    else
      conf_file = argv[1];
    
    if((fd = open(conf_file,O_RDONLY, S_IRUSR)) == -1)
      syserr("open(conf_file) failed.\n");
    
    if(argc == 1)
      free(conf_file);

    lines = count_lines(fd);    
    n_threads = read_integer(fd);
    available_workers = n_threads;
    remaining_work = lines;
    
    threads = (pthread_t *)malloc(n_threads*sizeof(pthread_t));
    
    thread_ids = (int *)malloc(lines*sizeof(int));    
  
    operations = (operation *)malloc(lines*sizeof(operation));
    thread_args = init_thread_args(n_threads, &remaining_work, operations, &available_workers);
    copy_operations(fd, thread_ids, &operations, lines);
 
    for(i = 0;i < n_threads;++i)
      if(pthread_create((threads+i),NULL,start,(void*)(thread_args +i)))
      	syserr("pthread_create() failed.\n");
     
    for(i = 0;i < lines;++i){
      thread_arg *cursor;
      id = thread_ids[i]-1;
      if(id == -1){
	pthread_mutex_lock(thread_args->global_mutex);
	sprintf(buf,"[MAIN] available_workers = %d\n", available_workers);
	print_to_video(buf);
	if(available_workers == 0){
	  sprintf(buf,"[MAIN] No workers available, main thread sleeps.\n");
	  print_to_video(buf);
	  pthread_cond_wait(thread_args->father_hold,thread_args->global_mutex);}
	pthread_mutex_unlock(thread_args->global_mutex);
	  
	j = 0;
	while(*(thread_args + j)->offset >= 0){
	  j = (j + 1) % n_threads;
	}
	id = j;
      }
      cursor = thread_args + id;
      pthread_mutex_lock(cursor->my_mutex);
      if(*cursor->offset >= 0){
	sprintf(buf, "[MAIN] waiting for thread\n");
	print_to_video(buf);
	pthread_cond_wait(cursor->synchronizer,cursor->my_mutex);
      }
      *cursor->offset = i;
      pthread_mutex_unlock(cursor->my_mutex);
      pthread_cond_signal(cursor->synchronizer);     
    }

    pthread_mutex_lock(thread_args->global_mutex);
    if(remaining_work){
      sprintf(buf,"[MAIN] waiting on %d threads still working\n", remaining_work);
      print_to_video(buf);
      pthread_cond_wait(thread_args->father_sync, thread_args->global_mutex);
    }
    pthread_mutex_unlock(thread_args->global_mutex);
    operations->op = 'K';
    
    for(i = 0;i < n_threads;++i){
      thread_arg *cursor;
      cursor = thread_args + i;
      pthread_mutex_lock(cursor->my_mutex);
      *cursor->offset = 0;
      pthread_mutex_unlock(cursor->my_mutex);
      pthread_cond_signal(cursor->synchronizer);   
    }

    for(i = 0; i < 1; ++i)
      if(pthread_join(*(threads+i),NULL))
	syserr("pthread_join() failed.\n");
	
    results = get_results(operations, lines);
    char *file_name = "res.txt";
    if((fd = open(file_name, O_WRONLY|O_CREAT, 0666)) == -1){
      perror("open res");
      exit(1);
    }
    
    for(i = 0; i < lines; ++i){
      char string[64];
      sprintf(string, "%f\n", *(results+i));
      print_to_file(fd, string, strlen(string));
    }
    
    exit(0);
  }
}
