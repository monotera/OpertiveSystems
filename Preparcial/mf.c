#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc,char *argv[]) {

    int num;
    pid_t pid;

    pid= fork();

    if (pid==0){

        for (num= 0; num< 3; num++) {

            pid= fork();
            printf ("Soy el proceso de PID %d y mi padre tiene%d de PID.\n", getpid(), getppid());
            if (pid== 0)
                break;
        }
    }
    return 0;
} 


/*#define MAXLON 1000

void *cuenta(void * nombre)
{
    int pos, cont = 0, leidos;
    char cadena[MAXLON];
    int fd;
    int mf;
    fd = open(nombre, O_RDONLY);
    while ((leidos = read(fd, cadena, MAXLON)) != 0)
        for (pos = 0; pos < leidos; pos++)
            if ((cadena[pos] == 'a') || (cadena[pos] == 'A'))
                cont++;
    printf("Fichero %p:%d caracteres ’a’ o ’A’ encontrados \n", nombre, cont);
    close(fd);
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    
    if (argc == 1)
    {
        printf("Indica el nombre de un fichero.\n");
        exit(0);
    }
    pthread_t thread1[argc];
    int *tarea_ids[argc];
    for (int i = 1; i < argc; i++)
    {
        pthread_create(&thread1[i], NULL,cuenta, (void *) argv[i]);
    }
    for (int i = 1; i < argc; i++)
    {
        pthread_join(thread1[i], NULL);
    }

    return 0;
}*/