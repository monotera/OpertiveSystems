#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
int nprocesos = 4;
int i, childpid;

   for (i = 0; i < nprocesos; i++) {
     if ((childpid = fork()) < 0) {
       perror("fork:");
       exit(1);
     } 
     // Codigo que ejecutaran los hijos
     printf("Soy el hijo %d y mi padre es %d\n",getpid(),getppid());
     if (childpid > 0)  //los padres se salen del ciclo,
                              // el hijo sigue en el for.
							          break; 
   }

} // main

