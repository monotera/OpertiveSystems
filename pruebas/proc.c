#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

pid_t id[10];

void signalHandler()
{

   printf("Signal Handler %d\n", getpid());
}

void func()
{
   signal(SIGUSR1, signalHandler);
   while (1)
   {
      printf("Soy el proceso hijo %d mi padre %d \n", getpid(), getppid());
      pause();
      printf("brrrrrrrrrrrrrrrr Soy el proceso hijo %d mi padre %d \n", getpid(), getppid());
   }
}

int main()
{
   int pid, status, x, y;
   int *pIdM, *pIdR;
   pIdM = (int *)calloc(10, sizeof(int));
   pIdR = (int *)calloc(5, sizeof(int));
   int fd;
   mode_t fifo_mode = S_IRUSR | S_IWUSR;
   unlink("aPipe"); // por si ya existe

   if (mkfifo("aPipe", fifo_mode) == -1)
   {
      perror("mkfifo");
      exit(1);
   }
   for (int i = 0; i < 10; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         int x = getpid();
         fd = open("aPipe", O_WRONLY);
         write(fd, &x, sizeof(int));
         close(fd);
         func();
         exit(0);
      }
      else if (pid < 0)
      {
         printf("Error");
         exit(-1);
      }
      else if (pid > 0)
      {
         int y;
         fd = open("aPipe", O_RDONLY);
         read(fd, &y, sizeof(int));
         pIdM[i] = y;
         //printf("%d\n", y);
         close(fd);
      }
   }
   for (int i = 0; i < 5; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         x = getpid();
         fd = open("aPipe", O_WRONLY);
         write(fd, &x, sizeof(int));
         close(fd);
         func();
         exit(0);
      }
      else if (pid < 0)
      {
         printf("Error");
         exit(-1);
      }
      else if (pid > 0)
      {
         y;
         fd = open("aPipe", O_RDONLY);
         read(fd, &y, sizeof(int));
         pIdR[i] = y;
         //printf("%d\n", y);
         close(fd);
      }
   }
   for (int i = 0; i < 10; i++)
   {
      printf("%d\n", pIdM[i]);
   }
   printf("Red");
   for (int i = 0; i < 5; i++)
   {
      printf("%d\n", pIdR[i]);
   }
   kill(pIdM[1], SIGUSR1);
   kill(pIdM[0], SIGUSR1);
   wait(&status);
}
