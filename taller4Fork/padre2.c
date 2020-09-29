#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
pid_t hijopid;
 
hijopid = fork();
if (hijopid == 0) {
   printf("Soy el hijo con id = %d y mi padre es =%d \n",getpid(),getppid());
   // código que ejecuta el hijo
   exit(0);
} else  {
    // >  0
    printf("Soy el padre con id = %d y mi padre es =%d \n",getpid(),getppid());
   
   // código que ejecuta el padre
   }
} // del main