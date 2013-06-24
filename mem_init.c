#include "pcalc.h"

int read_integer(int fd){
  char c[1], buf[64];
  char *temp = buf;
  
  if(read(fd,c,1) == -1 || *c == '\n') 
    syserr("read(conf_file) failed.\n");
  
  while(*c != '\n' && *c != ' '){
    
    if((*c >= '0' && *c <= '9') )
      *(temp++) = *c;
    else
      syserr("Configuration file contains bad data.\n");
    
    if(read(fd,c,1) == -1)
      syserr("read(conf_file) failed.\n");
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
	
  if(read(fd,c,1) == -1 || !is_operator(*c))
    syserr("Configuration file contains bad data.\n");
  operator = *c;
	
  if(read(fd,c,1) == -1 ) 
    syserr("read(conf_file) failed.\n");

  if(read(fd,c,1) == -1 || *c == '\n')
    syserr("Configuration file contains bad data.\n");

  while(*c != '\n' && *c != EOF){
    if((*c >= '0' && *c <= '9') )
      *(aux++) = *c;

    else
     syserr("Configuration file contains bad data.\n");

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

//obsolete??
void copy_operations(int fd, int *proc_id, operation **operations,int lines){
  int i = 0;
  char buf[64];
  operation * cursor = *operations;
  //printf("cops %p", cursor);
  while(lines--){
    proc_id[i++] = read_integer(fd);
    write_line(fd, cursor);		
    sprintf(buf,"\t%d %c %d\n",cursor->num1, cursor->op,cursor->num2);
    //    print_to_video(buf);
    cursor++;
  }

}
