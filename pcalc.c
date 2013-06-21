#include "pcalc.h"


float *get_results(operation *operations,int lines){
  int i;
  float *temp = (float*)malloc(lines*sizeof(float));
  for(i = 0; i < lines ; ++i){
    *(temp+i) = (operations+i)->res;
  }
  return temp;
}

void print_to_file(int fd, char *string, int len){
  int n;

  if((n = write(fd, string, len)) == -1){
    perror("write");
    exit(1);
  }
}

int count_lines(int fd){
  int chars, n, lines;
  char buf[1];
  chars = 0;
  lines = -1;
  while((n = read(fd,buf, 1)) > 0){//controllo per la sintassi
    chars++;
    if(*buf == '\n')
      lines++;
  }	
  
  lseek(fd, -chars, SEEK_END);
  return lines;
}

char *prompt_user(char *msg){
  int i = 0;
  char *filename = malloc(64*sizeof(char));
  char buf[1];
  //char *str = "Inserisci nome file di configurazione: ";
  write(1, msg, strlen(msg));
  fflush(stdout);
  while((read(2, buf, 1) > 0) && *buf != '\n'){
    filename[i++] = buf[0];
  }
  filename[i] = '\0';
  i = 0;	
  return filename;
}

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
  
  if(argc > 2){
    exit(1);
  }
  else{
    if(argc == 1){
      conf_file = prompt_user("insert file name: ");
    }
    
    else
      conf_file = argv[1];
    
    if((fd = open(conf_file,O_RDONLY, S_IRUSR)) == -1){
      perror("open conf");
      exit(1);
    }
    
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
 
    /*
      for(i = 0; i<lines; ++i){
      j = i;
      printf("%d %c %d\n",(operations +j)->num1, (operations+j)->op,(operations +j)->num2);
      }*/
    
    for(i = 0;i < n_threads;++i){
      if(pthread_create((threads+i),NULL,start,(void*)(thread_args +i))){
      	perror("nato morto\n");
      	exit(1);
      }
    }
    
    for(i = 0;i < lines;++i){
      thread_arg *cursor;
      id = thread_ids[i]-1;
      if(id == -1){
	pthread_mutex_lock(thread_args->global_mutex);
	printf("available_workers = %d\n", available_workers);
	if(available_workers == 0){
	  printf("SLEEPPOOOOOOOOO\n");
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
	printf("SLEEPPO NELLA PRIMA COND! %d\n",i);
	pthread_cond_wait(cursor->synchronizer,cursor->my_mutex);
      }
      *cursor->offset = i;
      pthread_mutex_unlock(cursor->my_mutex);
      pthread_cond_signal(cursor->synchronizer);     
    }

    pthread_mutex_lock(thread_args->global_mutex);
    if(remaining_work){
      printf("MI SLEEPPO NELLA COND remaining = %d\n", remaining_work);
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
      if(pthread_join(*(threads+i),NULL)){
	perror("join");
	exit(1);
      }
    exit(0);
  }
}
