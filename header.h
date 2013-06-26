/**
 * @file   header.h
 * @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
 * @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
 * @date   July, 2013
 * @brief  Header file.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct operation{
  int num1;
  char op;
  int num2;
  float res;
}operation;

typedef struct thread_arg{
  pthread_mutex_t *my_mutex;
  pthread_mutex_t *global_mutex;
  pthread_cond_t *synchronizer;
  pthread_cond_t *father_sync;
  pthread_cond_t *father_hold;
  operation *operations;
  int thread_id;
  int *offset;
  int *remaining_work;
  int *available_workers;
}thread_arg;

/**
 * @brief Reads an integer from the configuration file. 
 * @param fd the file descriptor of the configuration file.
 * @return the integer read.
 * 
 */
int read_integer(const int fd);

/**
 * @brief Stores all the operations from the configuration file. 
 * @param fd file descriptor of the configuration file.
 * @param operations pointer to the array of operations.
 * @param line number of operations.
 */
void copy_operations(const int fd, int *thread_id, operation **operations, int lines);

/**
 * @brief thread's only function, where they execute all computations in the configuration file 
 * @param arg arguments passed to the threads
 * 
 */
void *start(void *arg);

/**
 * @brief Prints a string on the stderr and exits with "exit code" 1.
 * @param str the string to print.
 */
void syserr(const char *str);

/**
 * @brief Writes in specific file the string.
 * @param fd file descriptor of the file.
 * @param string the string to write.
 * @param len length of the string.
 *
 */
void print_to_file(const int fd, const char *string, const int len);

/**
 * @brief Prints a string on the stdout.
 * @param str the string to print
 *
 */
void print_to_video(const char *str);

/**
 * @brief Initializes the arguments passed to the threads
 * @param n_thread number of threads to be initialized
 * @param lines pointer to the number of operations in the configuration file
 * @param operations array of all the operations read in the configuration file
 * @param available_workers pointer to the number of available workers
 *
 */
thread_arg *init_thread_args(const int n_threads, int *lines, operation *operations, int *available_workers);

/**
 * @brief Creates an array of the results of the operations.
 * @param operations pointer to the array of operations
 * @param lines number of operations.
 * @return pointer to the array of results.
 *
 */
float *get_results(operation *operations,const int lines);

/**
 * @brief Counts the number of operations in the configuration file.
 * @param fd file descriptor of the configuration file.
 * @return the number of operations.
 */
int count_lines(const int fd);

/**
 * @brief Reads the name of the configuration file from the stdin.
 * @param msg message printed on the sdout before reading.
 * @return strings which contains the name of the configuration file.
 */
char *prompt_user(const char *msg);
