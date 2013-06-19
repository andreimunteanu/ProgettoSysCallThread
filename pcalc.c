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
void *ciao(void *arg){
  char *c=(char*)arg;
  printf("\t %p %s \n",c,c);
  pthread_exit(NULL);
}
void main(int argc, char *argv[]){
  int fd,n_threads,j,i = 0, lines = -1;  // da dichiarare i register
  int available_workers, remaining_work;
  pthread_t *threads;
  int *thread_ids;
  float *results;
  operation *operations;
  thread_arg **thread_args;
  int id;
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
    available_workers = 2*n_threads;
    remaining_work = 2*n_threads +1;
    
    threads = (pthread_t*)malloc(n_threads*sizeof(pthread_t));
    
    thread_ids = (int *)malloc(lines * sizeof(int));    
	
    operations = (operation *) malloc(lines*sizeof(operation));
    
    //addresses = (op_addr)*malloc(n_thread*sizeof(op_addr));
    
    copy_operations(fd, thread_ids,  &operations,lines);
    
    thread_args = init_thread_args(n_threads,&lines);

    printf("%d \n",lines);
    for(i = 0; i<lines; ++i){
      j = i*sizeof(operation);
      printf("%d %c %d\n",(operations +j)->num1, (operations+j)->op,(operations +j)->num2);
    }
    thread_arg *temp;
    char a ='a';
    char* c="ciao";
   
    for(i = 0;i < n_threads;++i){
      temp = *(thread_args +i*sizeof(thread_arg*));
      
      printf("padre figlio %d %p %p \n",i,temp,temp->my_mutex);
      }
    for(i = 0;i < n_threads;++i){
      temp =*(thread_args +i*sizeof(thread_arg*));
      
        // printf("padre figlio %d %p %p \n",i,temp,temp->remaining_work);
       if(pthread_create(&threads[i],NULL,start,(void*)(temp))){
      	perror("nato morto\n");
      	exit(1);//??
	}
    }

    printf("Ho procreato\n");
    // tocca a babbo
    /* for(i= 0; i < lines; ++i){
      id = (thread_ids[i]-1);
      
      if(id == -1){

      }
      
      //  *(addresses + id*sizeof(op_addr)) = operations+i*sizeof(operation);
      }*/
    
    //tipo  wait_results(remaining_work);
    //    printf("i figli hanno finito, ora li coppo\n");
    /*
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

    operations[0].op = 'K';
    
    for(i = 0; i < n_threads; ++i){
    
    }
    
    */
    // void **thread_return;
     for(i = 0; i < 1; ++i)
       if(pthread_join(*(threads+i*sizeof(pthread_t)),NULL)){
	perror("join");
	exit(1);
	}

    for(i = 0;i < n_threads;++i){
      temp = *(thread_args +i*sizeof(thread_arg*));
      
      printf("padre figlio %d %p %p \n",i,temp,temp->my_mutex);
      }
    /*for(i = 0;i < n_threads;++i){
       temp = thread_args +i*sizeof(thread_arg);
      
      printf("padre figlio %d %p %p \n",i,temp,temp->remaining_work);
      }*/
     for(i = 0; i < n_threads; ++i)
       printf("%d\n", thread_ids[i]);

    printf("Ciao papa, muore.\n");
    // sleep(6);
    exit(0);
  }
}
