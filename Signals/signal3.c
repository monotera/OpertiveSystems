#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int  alarmFlag=0;
void signalHandler ()
{
  printf ("Signal received\n");
  alarmFlag=1; 
}

int main ()
{
 int pid, status;
 signal (SIGALRM, signalHandler);
 alarm(5); 
  while(!alarmFlag) {
       pause();
  }
  printf("Fin");
}  