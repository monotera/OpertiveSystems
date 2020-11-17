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

void signalHandlerFinisher()
{
   printf("Process %d ending\n", getpid());
   flag = 0;
}

int init(int *pIdM, int *pIdR, int nmappers, int nreducers)
{
   int fd, pid, idW, idR, i,res;
   char pipeName[100];
   mode_t fifo_mode = S_IRUSR | S_IWUSR;
   int *allocator[nreducers];
   signal(SIGUSR1, signalHandlerFinisher);
   unlink("pIdPipe");
   if (mkfifo("pIdPipe", fifo_mode) == -1)
   {
      perror("mkfifo");
      return -1;
   }

   assignPipes(nmappers, nreducers, allocator);

   for (i = 0; i < nreducers; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         idW = getpid();
         fd = open("pIdPipe", O_WRONLY);
         write(fd, &idW, sizeof(int));
         close(fd);
         reducer(i, allocator[i]);
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
         read(fd, &idR, sizeof(int));
         pIdR[i] = idR;
         close(fd);
      }
   }

   for (i = 0; i < nmappers; ++i)
   {
      pid = fork();
      if (pid == 0)
      {
         idW = getpid();
         fd = open("pIdPipe", O_WRONLY);
         write(fd, &idW, sizeof(int));
         close(fd);
         res = assignReducer(pIdR, nreducers, i, allocator);
         mapper(i, pIdR[res]);
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
         read(fd, &idR, sizeof(int));
         pIdM[i] = idR;
         close(fd);
      }
   }
   unlink("pIdPipe");

   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", pIdM[i]);
      unlink(pipeName);
      if (mkfifo(pipeName, fifo_mode) == -1)
      {
         perror("mkfifo");
         return -1;
      }
      sprintf(pipeName, "pipeMR%d", i);
      unlink(pipeName);
      if (mkfifo(pipeName, fifo_mode) == -1)
      {
         perror("mkfifo");
         return -1;
      }
   }
   return 0;
}
int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int *pIdM, int *pIdR)
{
   int status;
   status = sendCommand(command, nmappers, pIdM);
   if (status == MONE)
   {
      return -1;
   }
  /* kill(pIdM[0],SIGCONT);*/
   printf("volvi\n");
   return 0;
}

int sendCommand(char *commandI, int nmappers, int *pIdM)
{

   int fd, i;
   char pipeName[100];
   struct command com;

   com = transform_command(commandI);
   if (com.dif == MONE)
   {
      perror("Error: invalid command\n");
      deleteFiles(nmappers, "split");
      return -1;
   }

   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", pIdM[i]);
      kill(pIdM[i], SIGCONT);
      fd = open(pipeName, O_WRONLY);
      write(fd, &com, sizeof(struct command));
      close(fd);
   }

   return 0;
}

int mapper(int id, int redId)
{
   int fd;
   char pipeName[100];

   printf("HOlaM\n");
   while (flag)
   {
      kill(getpid(), SIGSTOP);
      struct command com;
      sprintf(pipeName, "pipeCom%d", getpid());
      fd = open(pipeName, O_RDONLY);
      read(fd, &com, sizeof(struct command));
      close(fd);
      printf("%d\n",redId);
   }
   printf("Adios\n");
}
int reducer(int id, int *abcd)
{
   int j = 0;
   printf("HOlaR %d\n",getpid());
   while (flag)
   {
      kill(getpid(), SIGSTOP);
   }
   printf("AdiosR\n");
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
struct command transform_command(char *command)
{
   struct command com;
   char *col;
   col = (char *)malloc(strlen(command));
   strcpy(col, command);
   char *token;
   token = strtok(col, ",");
   int place = 0;
   int colum = 0;
   char *dif;
   dif = (char *)malloc(3);
   int eq = 0;
   int flag = 0;

   while (token != NULL)
   {
      switch (place)
      {
      case 0:
         colum = atoi(token);
         break;
      case 1:
         strcpy(dif, token);
         break;
      case 2:
         if (strcmp(token, "0") == ZERO) /*Its 0 so the validation is already done*/
         {
            flag = 1;
         }
         eq = atof(token);
         break;

      default:
         break;
      }
      token = strtok(NULL, ",");
      place++;
   }

   int res = validate_command(colum, dif, eq, flag);

   if (res > ZERO)
   {
      com.col = colum;
      com.dif = res;
      com.eq = eq;
   }
   else
   {
      com.dif = -1;
   }
   return com;
}

int validate_command(int col, char *dif, int eq, int flag)
{
   int validation = -1;
   if (col <= ZERO || col > NT) /*Error message columns cant be negative an 0 means its a letter*/
   {
      return validation;
   }
   if (eq == ZERO && flag == ZERO)
   {
      return validation;
   }
   if (strcmp(dif, "<") == ZERO)
   {
      validation = 1;
   }
   else if (strcmp(dif, ">") == ZERO)
   {
      validation = 2;
   }
   else if (strcmp(dif, "=") == ZERO)
   {
      validation = 3;
   }
   else if (strcmp(dif, ">=") == ZERO)
   {
      validation = 4;
   }
   else if (strcmp(dif, "<=") == ZERO)
   {
      validation = 5;
   }
   return validation;
}
int deleteFiles(int canti, char *type)
{
   int i;
   char *aux = (char *)malloc(10);
   sprintf(aux, "%s%d.txt", type, 0);
   for (i = ZERO; i < canti; i++)
   {
      int rc = remove(aux);
      if (rc)
      {
         perror("The file couldnt be delete it");
         free(aux);
         return -1;
      }
      sprintf(aux, "%s%d.txt", type, i + 1);
   }
   free(aux);
   return 0;
}

int finalizer(int *pIdM, int nmappers)
{
   int i;
   char pipeName[100];
   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", pIdM[i]);
      unlink(pipeName);
      sprintf(pipeName, "pipeMR%d", i);
      unlink(pipeName);
   }
}

int assignReducer(int *pIdR, int nreducers, int index, int **allocator)
{
   int i;
   int j;

   for (i = 0; i < nreducers; i++)
   {
      j = 0;
      while (allocator[i][j] != -1)
      {
         if (allocator[i][j] == index)
         {
            return i;
         }
         j++;
      }
   }
}
int assignPipes(int nmappers, int nreducers, int **allocator)
{
   int i = 0;
   int k = 0;
   int j = 0;

   for (i = ZERO; i < nreducers; i++)
      allocator[i] = (int *)calloc(20, sizeof(int) * 20);
   i = 0;
   while (i < nmappers)
   {
      allocator[j][k] = i;
      allocator[j][k + 1] = -1;
      i++;
      j++;
      if (j == nreducers)
      {
         j = 0;
         k++;
      }
   }
   return 0;
}