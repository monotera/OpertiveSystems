#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{printf("hola");
     int fd[2];
     pipe(fd);
     dup2(fd[READ], 0);
     dup2(fd[WRITE], 1);
     close(fd[READ]);
     close(fd[WRITE]);
     pipe(fd);
     
     int y,x;
     if (fork() == 0)
     {
          // Esto lo hace el hijo
          dup2(fd[WRITE], 1);
          x = 2;
          write(fd[1], &x, sizeof(int));
     }
     else
     {
          dup2(fd[READ], 0); // Esto lo hace el padre
          read(fd[0], &y, sizeof(int));
          printf("hola %d",y);
     }
     close(fd[READ]);
     close(fd[WRITE]);
}
