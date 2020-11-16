/**
 * Name: libProcess.c
 * Operative systems first project
 * Authors: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Date: 4/oct/2020
 * Description: Implementation of the "libProcess.h" library.
 **/

#include "libProcess.h"

#define CHUNK 1024
#define ONE 1
#define ZERO 0
#define MONE -1
#define OTHER -163
#define NT 18

int flag = 1;

void signalHandler()
{
   printf("Signal Handler %d\n", getpid());
   flag = 0;
}
int init(int *pIdM, int *pIdR, int nmappers, int nreducers)
{
   int i;
   int fd, pid, x, y;
   mode_t fifo_mode = S_IRUSR | S_IWUSR;
   unlink("pIdPipe");
   if (mkfifo("pIdPipe", fifo_mode) == -1)
   {
      perror("mkfifo");
      return -1;
   }
   for (i = 0; i < nmappers; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         signal(SIGUSR1, signalHandler);
         x = getpid();
         fd = open("pIdPipe", O_WRONLY);
         write(fd, &x, sizeof(int));
         close(fd);
         mapper();
         exit(0);
      }
      else if (pid < 0)
      {
         printf("Error");
         exit(-1);
      }
      else if (pid > 0)
      {
         fd = open("pIdPipe", O_RDONLY);
         read(fd, &y, sizeof(int));
         pIdM[i] = y;
         close(fd);
      }
   }
   for (i = 0; i < nreducers; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         signal(SIGUSR1, signalHandler);
         x = getpid();
         fd = open("pIdPipe", O_WRONLY);
         write(fd, &x, sizeof(int));
         close(fd);
         reducer();
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
         fd = open("pIdPipe", O_RDONLY);
         read(fd, &y, sizeof(int));
         pIdR[i] = y;
         close(fd);
      }
   }
   unlink("pIdPipe");
   return 0;
}
int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int *pIdM, int *pIdR)
{
   int i;
   for (i = 0; i < nmappers; i++)
   {
      printf("%d\n", pIdM[i]);
   }
   for (i = 0; i < nreducers; i++)
   {
      printf("%d\n", pIdR[i]);
   }
   return 0;
}

mapper()
{
   while (flag)
   {
      pause();
   }
}
reducer()
{
   while (flag)
   {
      pause();
   }
}

int split(char *logfile, int lines, int nmappers)
{

   FILE *file;
   int sub_lines = 0;
   int cont_lines = 0;
   int cont_splitFer = 0;
   int flag = 0;

   char *str;
   char *aux;

   file = fopen(logfile, "r");
   sub_lines = ((lines) / (nmappers));

   if (file != NULL && sub_lines > ZERO)
   {

      char *str = (char *)malloc(CHUNK);
      int cont_lines = 1;
      int cont_splitFer = 0;
      char *aux = (char *)malloc(11);
      strcpy(aux, "split0.txt");
      FILE *writer;
      flag = 1;

      while (fgets(str, CHUNK, file))
      {
         if (flag == ONE)
         {
            writer = fopen(aux, "w");
            if (writer == NULL)
            {
               perror("Error: Writer couldn't be open\n");
               return -1;
            }
            flag = 0;
         }
         if (cont_lines == sub_lines && cont_splitFer != nmappers - ONE)
         {
            cont_lines = 0;
            cont_splitFer++;
            sprintf(aux, "split%d.txt", cont_splitFer);
            flag = 1;
            fprintf(writer, "%s", str);
            fclose(writer);
         }
         else
         {
            fprintf(writer, "%s", str);
         }

         cont_lines++;
      }
      fclose(file);
      fclose(writer);

      free(str);
      free(aux);
   }
   else
   {
      perror("Error: File couldn't be open\n");
      return -1;
   }
   return 0;
}
int validationParameters(char *log, int lines, int nmappers, int nreducers)
{
   int fileLines;
   fileLines = lineCounter(log);
   fileLines++;
   int fileLinesBlank = fileLines - 1;

   FILE *fp = fopen(log, "r");
   if (fp == NULL)
   {
      printf("The file doesnt exist!");
      return -1;
   }
   fclose(fp);
   if (fileLines != lines && fileLinesBlank != lines)
   {
      printf("Invalid number of lines\n");
      return -1;
   }
   if (nreducers <= ZERO || nreducers > nmappers)
   {
      perror("Invalid number of reducers\n");
      return -1;
   }
   if (nmappers <= ZERO || nmappers > lines)
   {
      printf("Invalid number of mappers\n");
      return -1;
   }

   return 0;
}
int lineCounter(char *log)
{
   char *command;
   command = (char *)calloc(20, sizeof(log) * 20);
   FILE *fp1;
   int lines;
   sprintf(command, "cat %s | wc -l > lineCounterAux.txt", log);
   system(command);
   fp1 = fopen("lineCounterAux.txt", "r");
   if (fp1 == NULL)
   {
      perror("Error: ");
      return (-1);
   }
   fscanf(fp1, "%d", &lines);
   fclose(fp1);
   remove("lineCounterAux.txt");
   free(command);
   return lines;
}