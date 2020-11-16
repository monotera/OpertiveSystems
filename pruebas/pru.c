#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

sem_t mutex, slots, items, global, global2;
#define SLOTS 2
#define ITEMS 10
int x = 0;
int pid[10];

void signalHandler()
{
   printf("Signal Handler %d\n", getpid());
}

void *produce(void *arg)
{
   signal(SIGUSR1, signalHandler);
   int *id_ptr, tarea_id;
   id_ptr = (int *)arg;
   tarea_id = *id_ptr;
   pid[tarea_id] = pthread_self();
   printf("producer %d\n", tarea_id);
   sem_wait(&global);
   printf("producerbye %d\n", tarea_id);

   return NULL;
}

void *consume(void *arg)
{
   printf("consume\n");
   pause();
   printf("consumebye\n");
   return NULL;
}

int main()
{
   pthread_t tcons[10], tpro[10];
   printf("Inicia el programa\n");
   sem_init(&mutex, 0, 1);
   sem_init(&global, 0, 0);
   sem_init(&global2, 0, 0);
   sem_init(&slots, 0, SLOTS);
   sem_init(&items, 0, 0);
   int *tarea_ids[10];
   for (int i = 0; i < 10; i++)
   {
      tarea_ids[i] = (int *)malloc(sizeof(int));
      *tarea_ids[i] = i;
      pthread_create(&tpro[i], NULL, produce, (void *)tarea_ids[i]);
      pthread_create(&tcons[i], NULL, consume, NULL);
   }
   for (int i = 0; i < 10; i++)
   {
      sem_post(&global);
   }

   for (int i = 0; i < 10; i++)
   {
      pthread_join(tcons[i], NULL);
      pthread_join(tpro[i], NULL);
   }

   printf("\n%d\n", x);
   sem_destroy(&mutex);
   sem_destroy(&slots);
   sem_destroy(&items);
   printf("Fin del programa\n");
   return 0;
}