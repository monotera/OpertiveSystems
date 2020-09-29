#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
pid_t hijopid;
 
hijopid = fork();
if (hijopid == 0) {
   printf("Soy el hijo \n");
   // código que ejecuta el hijo
   exit(0);
} else  {
    // >  0
    printf("Soy el padre \n");
   // código que ejecuta el padre
   }
} // del main