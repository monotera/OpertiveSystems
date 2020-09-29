#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/time.h>
#include  <pthread.h>


#define NUM_THREADS 3

char *mensaje[NUM_THREADS];
void *PrintHello(void *thread_id){
   int *id_ptr, tarea_id;
   id_ptr = (int *) thread_id;
   tarea_id = *id_ptr;
   printf("Thread %d: %s \n", tarea_id, mensaje[tarea_id]);
   pthread_exit(NULL);
}


int main(){

   pthread_t threads[NUM_THREADS]; // arreglo de identificadores
   int *tarea_ids[NUM_THREADS]; // apuntadores para pasar los parametros.
   int rc, t;
   mensaje[0] = "Ingles: Hello World!\n";
   mensaje[1] = "                           Espanol: Hola Mundo!\n";
   mensaje[2] = "                                                    Italiano: Ciao Mondo!\n";
   for(t=0;t<NUM_THREADS;t++){ 
      tarea_ids[t] = (int *) malloc(sizeof(int));
      *tarea_ids[t] = t;
      printf("Creando el thread %d \n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *) tarea_ids[t]);
      if (rc) {
         perror("Hello World");
         exit(-1);
      }
          

   }
   for(t=0;t<NUM_THREADS;t++){
      pthread_join(threads[t], NULL);
      }  
   // exit(0);
}
