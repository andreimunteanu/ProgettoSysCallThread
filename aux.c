#include "pcalc.h"

void syserr(char *str){
  write(3,str,strlen(str));
  exit(1);
}

void print_to_file(int fd, char *string, int len){
  int n;

  if((n = write(fd, string, len)) == -1)
    syserr("Write failed in print_to_file()");
}

void print_to_video(char *str){
  if(write(1,str,strlen(str)) < 0)
    syserr("print_to_video");
  fflush(stdout);
}

float *get_results(operation *operations,int lines){
  int i;
  float *temp = (float*)malloc(lines*sizeof(float));
  for(i = 0; i < lines ; ++i){
    *(temp+i) = (operations+i)->res;
  }
  return temp;
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
