#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int i;
void signalHandler ()
{
  printf ("Signal Handler\n");
  i++; 
}

int main ()
{
 int pid, status;
  signal (SIGALRM, signalHandler);
  printf("Instalado el manejador\n");
  alarm(5);
   while(1);
}  