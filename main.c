/**
* @mainpage
* @author <a href="mailto:niccolo.marastoni@studenti.univr.it">Niccolò Marastoni</a>
* @author <a href="mailto:andrei.munteanu@studenti.univr.it">Andrei Munteanu</a>
* @section Specifica
* Il programma dovrà leggere un file di configurazione, contenente:
* <ol>
* <li> Il numero di thread di calcolo parallelo.
* <li> I dati di computazione.
* </ol>
* Il file di configurazione avra' la seguente struttura:
* <ul>
* <li> La prima riga conterra' un intero, che corrisponde al numero di thread di calcolo parallelo da
creare, NTHREAD.
* <li> Le altre righe avranno il seguente formato: <id> <num1> <op> <num2>, dove: <br>
* <ol>
* <li> id e' il numero di thread di calcolo parallelo a cui far eseguire l'operazione, se e' maggiore o uguale a 1. 
* Se e' zero, indica che l'operazione dovra' essere eseguita dal primo thread libero.
* Si assuma che id corrisponda sempre ad un valore corretto.
* <li> num1 e num2 sono due numeri interi, corrispondenti ai dato sui quali eseguire
* l'operazione.
* <li> op e' un carattere rappresentante l'operazione da eseguire sui due numeri. Caratteri validi
* per op sono: '+', '-', '*', '/', rappresentanti rispettivamente la somma, la sottrazione, la
* moltiplicazione e la divisione.
* </ol></ul>
* Ogni thread di calcolo parallelo eseguirà una routine di questo tipo:
* <ol>
* <li> Attesa su un mutex per eseguire un calcolo. 
* <li> Ricezione dei dati e dell'operazione da eseguire da parte del thread principale.
* <li> Se riceve il comando di terminazione, termina la sua esecuzione. Il comando di terminazione e' indicato dal carattere 'K'.
* <li> Esecuzione del calcolo.
* <li> Invio del risultato al thread principale, e segnalazione al thread principale che ha finito di calcolare.
* <li> Attesa sullo stesso mutex del punto 1, per attendere il comando successivo.
* </ol>
* Il thread principale eseguira' in questo modo:
* <ol>
* <li> Setup della simulazione, leggendo dal file di configurazione il numero di processori da simulare, creandone i thread di calcolo parallelo relativi, 
* e creando ed inizializzando le eventuali strutture di supporto per la comunicazione. 
* Inoltre, verra' creato un array in cui memorizzare i risultati delle operazioni eseguite. 
* La lunghezza di tale array sara' ricavata dal numero di operazioni lette dal file di configurazione.
* <li> Entrata in un ciclo che per ogni operazione da simulare effettua quanto segue:
* <ol><li> Se il comando ha id diverso da zero, attende che il thread di calcolo parallelo numero id sia
* libero, salva il risultato dell'eventuale calcolo precedente nell'array dei risultati, e poi
* interagisce con lui passandogli il comando da simulare. Il thread principale non deve
* attendere che il processore abbia completato la simulazione dell'operazione passata al figlio.
* <li> Se l'istruzione ha id 0, trova il primo processore libero ed in caso interagisce con esso, come al punto 1. 
* Altrimenti attende che almeno un processore sia libero, e poi interagisce con esso come al punto 1.</ol>
* <li> Passati tutti i comandi ai thread di calcolo parallelo, attende che i thread abbiano eseguito tutti i loro calcoli.
* <li> Salva nell'array dei risultati gli ultimi risultati computati, e fa terminare i thread di calcolo
* parallelo passando loro il comando di terminazione 'K'.
* <li> Attende che tutti i thread abbiano terminato.
* <li> Stampa su un file di output tutti i risultati.
* <li> Libera eventuali risorse.
* <li> Esce
* </ol>
*/
#include "header.h"

void main(int argc, char *argv[]){
  register int j, i = 0;
  int fd,n_threads, lines = -1;  // da dichiarare i register
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

    free(threads);
    free(thread_ids);
    free(operations);
    free(thread_args->offset);
    free(thread_args->father_hold);
    free(thread_args->father_sync);
    free(thread_args->synchronizer);
    free(thread_args->global_mutex);
    free(thread_args->my_mutex);
    free(thread_args);
    
    exit(0);
  }
}
