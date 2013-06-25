/**
 * @file   mem_init.c
 * @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
 * @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
 * @date   July, 2013
 * @brief  Memory operations
 * 
 */
#include "header.h"


int read_integer(const int fd){
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

/**
 * @brief checks to see if a certain character is among the allowed list of operators
 * @param c the character to be checked
 * @return 0 if it's not an operator, an 
 */

int is_operator(char c){
  return (c == '+' || c == '-' || c == '*' || c == '/');
}

/**
 * @brief reads the rest of the line
 * @param fd file descriptor for the configuration file
 * @param cursor a pointer to the operation
 */
void read_remaining_line(const int fd, operation *cursor){
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

/**
 * @brief reads the first number in the operation and writes it in the first field of the structure
 * @param fd file descriptor for the configuration file
 * @param cursor a pointer to the operation
 */
void write_line(int fd, operation *cursor){
  cursor->num1 = read_integer(fd);
  read_remaining_line(fd, cursor);	
}

void copy_operations(const int fd, int *thread_id, operation **operations,int lines){
  register int i = 0;
  operation *cursor = *operations;
  
  while(lines--){
    thread_id[i++] = read_integer(fd);
    write_line(fd, cursor);		
    cursor++;
  }

}
