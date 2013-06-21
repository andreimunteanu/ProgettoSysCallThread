#include "pcalc.h"

int read_integer(int fd){
  char c[1], buf[64];
  char *temp = buf;
  
  if(read(fd,c,1) == -1 || *c == '\n'){
    perror("read");
    printf("codifica1\n");
    exit(1);
  }
  
  while(*c != '\n' && *c != ' '){
    
    if((*c >= '0' && *c <= '9') )
      *(temp++) = *c;
    else{
      printf("codifica2\n");
      exit(1);
    }
    
    if(read(fd,c,1) == -1){
      perror("read");
      exit(1);
    }
  }
  *temp = '\0';
  return atoi(buf);
}	

int is_operator(char c){
  return (c == '+' || c == '-' || c == '*' || c == '/');
}

void read_remaining_line(int fd, operation *cursor){
  char c[1], buf[64], operator;
  char *aux = buf;
	
  if(read(fd,c,1) == -1 || !is_operator(*c)){
    printf("bitch\n");
    exit(1);
  }
  operator = *c;
	
  if(read(fd,c,1) == -1 ) // da mettere a novanta
    exit(1);

  if(read(fd,c,1) == -1 || *c == '\n')
    exit(1);

  while(*c != '\n' && *c != EOF){
		
    if((*c >= '0' && *c <= '9') )
      *(aux++) = *c;
    else
      exit(1);

    if(read(fd,c,1) == -1)
      exit(1);
  }
  *aux = '\0';
  cursor->op = operator;
  cursor->num2 = atoi(buf);	
}

void write_line(int fd, operation *cursor){
  cursor->num1 = read_integer(fd);
  read_remaining_line(fd, cursor);	
}

void copy_operations(int fd, int *proc_id, operation **operations,int lines){
  int i = 0;
  char buf[64];
  operation * cursor = *operations;
  //printf("cops %p", cursor);
  while(lines--){
    proc_id[i++] = read_integer(fd);
    write_line(fd, cursor);		
    printf("\t%d %c %d\n",cursor->num1, cursor->op,cursor->num2);
    cursor++;
  }

}
