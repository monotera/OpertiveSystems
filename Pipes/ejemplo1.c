#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char **argv)
{
     int fd[2];
     int countbytes;
     char message[100];
     char frase[]="abc";
     pipe(fd);

     if (fork()==0) { // Codigo del hijo
          close(fd[0]);
          write(fd[1], frase, strlen(frase)+1);
          close(fd[1]);
          }
     else {     // Codigo del Padre
          close(fd[1]);
          countbytes = read(fd[0], message, 100);
          printf("Mensaje leido %s \n",message);
          close(fd[0]);
          }
}
