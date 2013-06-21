#include "pcalc.h"


float *get_results(operation *operations,int lines){
  int i;
  float *temp = (float*)malloc(lines*sizeof(float));
  for(i = 0; i < lines ; ++i){
    *(temp+i) = (operations+i*sizeof(operation))->res;
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
  pthread_t *threads;
  int *thread_ids;
  float *results;
  operation *operations;
  thread_arg *thread_args;
  int id;
  char *conf_file;
  char *filename;
  pthread_attr_t attr;
  
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
    available_workers = 2*n_threads;
    remaining_work = 2*n_threads +1;
    
    threads = (pthread_t *)malloc(n_threads*sizeof(pthread_t));
    
    thread_ids = (int *)malloc(lines*sizeof(int));    
	
    operations = (operation *)malloc(lines*sizeof(operation));
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    thread_args = init_thread_args(n_threads, &lines);
    copy_operations(fd, thread_ids, &operations, lines);
 
    /*
      for(i = 0; i<lines; ++i){
      j = i*sizeof(operation);
      printf("%d %c %d\n",(operations +j)->num1, (operations+j)->op,(operations +j)->num2);
      }*/

    thread_arg *temp;
    for(i = 0;i < n_threads;++i){
      temp = thread_args +i*sizeof(thread_arg);
      printf("PADRE THREAD ARGS: thread_args = %p\tmy_mutex = %p\tremaining_Work = %p\n******************************************************\n", temp, temp->my_mutex, temp->remaining_work);
      
     
    }
    
    for(i = 0;i < n_threads;++i){
      if(pthread_create((threads+i*sizeof(pthread_t)),&attr,start,(void*)(thread_args +i*sizeof(thread_arg)))){
      	perror("nato morto\n");
      	exit(1);
      }
    }
     
    sleep(2);
    for(i = 0;i < n_threads;++i){
      temp = thread_args +i*sizeof(thread_arg);
      printf("SECONDO PADRE THREAD ARGS: thread_args = %p\tmy_mutex = %p\tremaining_Work = %p\n******************************************************\n", temp, temp->my_mutex, temp->remaining_work); 
    }
    pthread_exit(NULL);
  }
}
