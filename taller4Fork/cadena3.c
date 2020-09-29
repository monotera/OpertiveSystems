  GNU nano 4.8                                                              cadena3.c                                                                         
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
int nprocesos = 5;
int i, childpid, status;

 
   for (i = 0; i < nprocesos; ++i) {
     if ((childpid = fork()) < 0) {
       perror("fork:");
       exit(1);
     } 
     // Codigo que ejecutaran los hijos
    if (childpid > 0) 
        break; 
   }
   while ((childpid = wait(&status)) != -1);
    printf("Soy el hijo con pid %d, Mi padre es %d\n", getpid(), getppid());

} // main
