#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
   int fd, messagelen, i;
   char message[100];
   sprintf(message, "Hello from PID %d", getpid());
   messagelen = strlen(message);
   do
   {
      fd = open("aPipe", O_WRONLY | O_NONBLOCK);
      if (fd == -1)
         sleep(1);
   } while (fd == -1);
   for (i = 1; i < 11; i++)
   {
      write(fd, message, messagelen);
      printf("Sending throught the pipe \n");
     /* sleep(3);*/
   }
   close(fd);
}
