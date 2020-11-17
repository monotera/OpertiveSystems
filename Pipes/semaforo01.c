#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define loop 100000

void *suma(void *arg);
void *resta(void *arg);
int counter = 0;
sem_t sem1,sem2;

int main(void)
{

    pthread_t thread1, thread2;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);
    pthread_create(&thread2, NULL, *resta, NULL);
    pthread_create(&thread1, NULL, *suma, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    /*printf("%d",counter);*/
}

void *suma(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(&sem1);
        /*counter+=1;*/
        printf("A");
        sleep(2);
        sem_post(&sem2);
    }
    pthread_exit(NULL);
}
void *resta(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        
        /*counter-=1;*/
        printf("B");
        sleep(1);
        sem_post(&sem1);
        sem_wait(&sem2);
        
    }
    pthread_exit(NULL);
}