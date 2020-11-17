#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    /*int fd[2];
    pid_t childpid;
    pipe(fd);
    dup2(fd[READ],0);
    dup2(fd[WRITE],1);
    close(fd[READ]);
    close(fd[WRITE]);
    pipe(fd);
    if(fork() == 0){
        dup2(fd[WRITE],1);
    }
    else
    {
        dup2(fd[READ],0);
    }
    close(fd[READ]);
    close(fd[WRITE]);*/

    int i;
    pid_t childpid;
    int nprocesos = 4;
    int fd[2];
    for (i = 0; i < nprocesos; ++i)
    {
		printf("iter\n");
        pipe(fd);
        dup2(fd[READ], 0);
        dup2(fd[WRITE], 1);
        close(fd[READ]);
        close(fd[WRITE]);
       /* pipe(fd);*/
        if ((childpid = fork()) < 0)
        {
            perror("fork:");
            exit(1);
        }
        // Codigo que ejecutaran los hijos
        if (childpid == 0)
        {
            printf("Soy el hijo con pid %d y mi padre es %d\n", getpid(), getppid());
			pipe(fd);
        }
        else
            break;
    } //fork
}
