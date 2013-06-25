/**
 * @file   aux.c
 * @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
 * @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
 * @date   July, 2013
 * @brief  Provides functions
 * 
 */
#include "header.h"

void syserr(const char *str){
  write(3,str,strlen(str));
  exit(1);
}

void print_to_file(const int fd,const char *string, const int len){
  int n;

  if((n = write(fd, string, len)) == -1)
    syserr("Write failed in print_to_file()");
}

void print_to_video(const char *str){
  if(write(1,str,strlen(str)) < 0)
    syserr("print_to_video");
  fflush(stdout);
}

float *get_results(operation *operations,const int lines){
  register int i;
  float *temp = (float*)malloc(lines*sizeof(float));
  for(i = 0; i < lines ; ++i){
    *(temp+i) = (operations+i)->res;
  }
  return temp;
}

int count_lines(const int fd){
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

char *prompt_user(const char *msg){
  int i = 0;
  char *filename = malloc(64*sizeof(char));
  char buf[1];
  write(1, msg, strlen(msg));
  fflush(stdout);
  while((read(2, buf, 1) > 0) && *buf != '\n'){
    filename[i++] = buf[0];
  }
  filename[i] = '\0';
  i = 0;  
  return filename;
}
