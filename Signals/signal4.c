#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

void signalHandler ()
{
  printf ("Signal Handler\n");
}


int main ()
{
int pid, status;
  signal (SIGUSR1, signalHandler);
  printf("Instalado el manejador\n");
  pid = fork ();
  if (!pid) {  
      pause();
      printf("Hijo Termina\n");
	  exit(0);
  }
  sleep (3); 
  kill (pid, SIGUSR1);
  wait (&status);
  printf ("El padre Termina, El hijo termina con exit code %d \n", status);
  
}  
