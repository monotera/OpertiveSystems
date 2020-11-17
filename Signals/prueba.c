#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int bandera = 1;
void signalHandler()
{
    printf("Signal Handler\n");
}

void fun(int p, int fd)
{
    
    char message[100];
    while (read(fd, message, 100) > 0)
    {
        printf("%s", message);
    }
    printf("Soy el mapper %d\n", p);
}

void fun2(int p, int fd)
{
    int i, creado;
    char message[100];
    int messagelen;
    sprintf(message, "%s %d\n", "el id = ", getpid());
    messagelen = strlen(message);
    write(fd, message, messagelen);
    printf("Soy el reduccer %d\n", p);
}

int main()
{
    int status;
    pid_t pidE;
    signal(SIGUSR1, signalHandler);
    printf("Instalado el manejador\n");
    int map = 1;
    int fd, creado, fd2;
    mode_t fifo_mode = S_IRUSR | S_IWUSR;
    unlink("aPipe"); // por si ya existe

    if (mkfifo("aPipe", fifo_mode) == -1)
    {
        perror("mkfifo");
        exit(1);
    }

    for (int i = 0; i < 1; i++)
    {
        pidE = fork();
        if (pidE < 0)
        {
            perror("fork:");
            exit(1);
        }
        else if (pidE == 0)
        {
            fd = open("aPipe", O_WRONLY | O_NONBLOCK);
            fun2(getpid(), fd);
            fd = open("aPipe", O_RDONLY);
            fun(getpid(), fd);
            close(fd);
            exit(0);
        }
        else
        {
            continue;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        pidE = wait(&status);
    }
}
