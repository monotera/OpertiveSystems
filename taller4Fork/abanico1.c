#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {

   int status, i,nprocesos;
   pid_t childpid;
   nprocesos = 4;
   for (i = 0; i < nprocesos; ++i) {
     if ((childpid = fork()) < 0) {
       perror("fork:");
       exit(1);
     } 
     // Codigo que ejecutaran los hijos
     if (childpid == 0) { 
       printf("Soy el hijo con pid %d y mi padre es %d\n", getpid(),getppid());
       exit(0); 
     }
     else printf("Soy el padre con el pid %d y mi padre es %d\n",getpid(),getppid());
   } //fork
   
} // main