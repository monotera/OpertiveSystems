#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main ()
{
  int pid, status;
  alarm(5);
  printf("Looping forever\n");
   while(1);
   printf("This line should never be executed\n");
}  