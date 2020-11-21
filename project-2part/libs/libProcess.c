/**
 * Name: libProcess.c
 * Operative systems second project
 * Authors: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Date: 19/nov/2020
 * Description: Implementation of the "libProcess.h" library.
 **/

#include "libProcess.h"

#define CHUNK 1024
#define ONE 1
#define ZERO 0
#define MONE -1
#define OTHER -163
#define NT 18

void signalHandlerMapper()
{
   exit(0);
}

int init(int *pIdM, int *pIdR, int nmappers, int nreducers, char *log, int lines, int inter)
{
   int fd, pid, idW, idR, i, res;
   char pipeName[100];
   char splitName[100];
   mode_t fifo_mode = S_IRUSR | S_IWUSR;
   int *allocator[nreducers];

   signal(SIGUSR1, signalHandlerMapper);

   /*Creates split files*/
   int status = split(log, lines, nmappers);
   if (status)
   {
      return status;
   }

   /*Creates named pipes*/
   unlink("pIdPipe");
   if (mkfifo("pIdPipe", fifo_mode) == -1)
   {
      perror("mkfifo");
      return -1;
   }

   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", i);
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
   for (i = 0; i < nreducers; i++)
   {
      sprintf(pipeName, "masterPipe%d", i);
      unlink(pipeName);
      if (mkfifo(pipeName, fifo_mode) == -1)
      {
         perror("mkfifo");
         return -1;
      }
   }

   /*create mappers and reducers*/
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
         mapper(i, pIdR[res], inter);
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
   return 0;
}
int processControl(int nmappers, int nreducers, char *command, int *pIdM, int *pIdR)
{
   struct timeval start;
   struct timeval end;
   gettimeofday(&start, NULL);
   int status;
   int fd, rd;
   int ans = 0;
   int i = 0;
   char pipeName[100];

   status = sendCommand(command, nmappers, pIdM);
   if (status == MONE)
   {
      return -1;
   }

   /* Receive response from reducers*/
   while (i < nreducers)
   {
      sprintf(pipeName, "masterPipe%d", i);
      fd = open(pipeName, O_RDONLY);
      read(fd, &rd, sizeof(int));
      close(fd);
      ans += rd;
      i++;
   }
   gettimeofday(&end, NULL);
   printf("Time of execution: %ld ms\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
   return ans;
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
      return -1;
   }

   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", i);
      kill(pIdM[i], SIGCONT);
      fd = open(pipeName, O_WRONLY);
      write(fd, &com, sizeof(struct command));
      close(fd);
   }
   return 0;
}

int mapper(int id, int redId, int inter)
{
   int fd;
   char pipeName[100];
   char splitName[100];
   sprintf(splitName, "split%d.txt", id);
   int status;
   struct command com;
   FILE *writer;
   while (ONE)
   {
      int i = 0;
      kill(getpid(), SIGSTOP);

      /*Recive command from master*/
      sprintf(pipeName, "pipeCom%d", id);
      fd = open(pipeName, O_RDONLY);
      read(fd, &com, sizeof(struct command));
      close(fd);

      struct map *buff;
      buff = (map *)calloc(CHUNK, sizeof(map) * CHUNK);
      if (buff == NULL)
      {
         perror("There was a problem allocating memory\n");
         return -1;
      }

      /*fills mappers to send*/
      status = findMatch(splitName, com, id, buff);
      if (status)
      {
         return -1;
      }
      /*Sends data to the reducers*/
      sprintf(pipeName, "pipeMR%d", id);
      do
      {
         kill(redId, SIGCONT);
         fd = open(pipeName, O_WRONLY | O_NONBLOCK);
      } while (fd == -1);
      if (inter == 1)
      {
         char *buffFile = (char *)calloc(20, sizeof(char) * 20);
         if (buffFile == NULL)
         {
            perror("There was a problem allocating memory\n");
            return -1;
         }
         sprintf(buffFile, "buff%d.txt", id);
         writer = fopen(buffFile, "w");
         if (writer == NULL)
         {
            perror("The file couldn't be open\n");
            inter = 0;
         }
      }
      while (buff[i].key != -1)
      {
         write(fd, &buff[i], sizeof(struct map));
         if (inter == 1)
            fprintf(writer, "%d %lf \n", buff[i].key, buff[i].value);
         i++;
      }
      write(fd, &buff[i], sizeof(struct map));
      if (inter == 1)
         fclose(writer);
      close(fd);
      free(buff);
   }
   printf("fin\n");
   return 0;
}
int findMatch(char *split, command com, int iter, map *maps)
{
   map x;
   int i = 0;
   char *buff = (char *)calloc(20, sizeof(char) * 20);
   if (buff == NULL)
   {
      perror("There was a problem allocating memory\n");
      return -1;
   }

   FILE *file = fopen(split, "r");
   if (file != NULL)
   {
      int h = 0;
      h = com.col;
      double buf[19];

      while (fscanf(file, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                    &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6],
                    &buf[7], &buf[8], &buf[9], &buf[10], &buf[11], &buf[12],
                    &buf[13], &buf[14], &buf[15], &buf[16], &buf[17], &buf[18]) != EOF)
      {
         x.value = -163;
         x.key = buf[1];
         switch (com.dif)
         {
         case 1:
            if (buf[h] < com.eq)
            {
               x.value = buf[h];
               maps[i].key = buf[1];
               maps[i].value = buf[h];
               i++;
            }
            break;
         case 2:
            if (buf[h] > com.eq)
            {
               x.value = buf[h];
               maps[i].key = buf[1];
               maps[i].value = buf[h];
               i++;
            }
            break;
         case 3:
            if (buf[h] == com.eq)
            {
               x.value = buf[h];
               maps[i].key = buf[1];
               maps[i].value = buf[h];
               i++;
            }
            break;
         case 4:
            if (buf[h] >= com.eq)
            {
               x.value = buf[h];
               maps[i].key = buf[1];
               maps[i].value = buf[h];
               i++;
            }
            break;
         case 5:
            if (buf[h] <= com.eq)
            {
               x.value = buf[h];
               maps[i].key = buf[1];
               maps[i].value = buf[h];
               i++;
            }
            break;
         default:
            break;
         }
      }
      maps[i].key = -1;
   }
   else
   {
      return -1;
   }
   fclose(file);
   return 0;
}
int reducer(int id, int *pipesId)
{
   int i = 0, cont = 0;
   int fd;
   char pipeName[100];
   struct map mapp;
   while (ONE)
   {
      if (pipesId[i] == -1)
      {
         /*waits until it has nothing more to read and then sends data to master*/
         sprintf(pipeName, "masterPipe%d", id);
         fd = open(pipeName, O_WRONLY);
         write(fd, &cont, sizeof(int));
         close(fd);
         i = 0;
         cont = 0;
      }
      mapp.key = 0;
      kill(getpid(), SIGSTOP);

      sprintf(pipeName, "pipeMR%d", pipesId[i]);
      fd = open(pipeName, O_RDONLY);
      while (mapp.key != -1)
      {
         read(fd, &mapp, sizeof(struct map));
         if (mapp.key != -1)
         {
            cont++;
         }
      }
      close(fd);
      i++;
   }
   printf("fin\n");
   return 0;
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
      if (str == NULL)
      {
         perror("There was a problem allocating memory\n");
         return -1;
      }
      int cont_lines = 1;
      int cont_splitFer = 0;
      char *aux = (char *)malloc(11);
      if (aux == NULL)
      {
         perror("There was a problem allocating memory\n");
         return -1;
      }
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
int validationParameters(char *log, int lines, int nmappers, int nreducers, int inter)
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
   if (inter != 0 && inter != 1)
   {
      printf("Invalid number of inter\n");
      return -1;
   }
   return 0;
}
int lineCounter(char *log)
{
   char *command;
   command = (char *)calloc(20, sizeof(log) * 20);
   if (command == NULL)
   {
      perror("There was a problem allocating memory\n");
      return -1;
   }
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
   if (col == NULL)
   {
      perror("There was a problem allocating memory\n");
      com.dif = -1;
      return com;
   }
   strcpy(col, command);
   char *token;
   token = strtok(col, ",");
   int place = 0;
   int colum = 0;
   char *dif;
   dif = (char *)malloc(3);
   if (dif == NULL)
   {
      perror("There was a problem allocating memory\n");
      com.dif = -1;
      return com;
   }
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
   if (aux == NULL)
   {
      perror("There was a problem allocating memory\n");
      return -1;
   }
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

int finalizer(int nmappers, int nreducers, int inter)
{
   int i, status;
   char pipeName[100];

   for (i = 0; i < nmappers; i++)
   {
      sprintf(pipeName, "pipeCom%d", i);
      unlink(pipeName);
      sprintf(pipeName, "pipeMR%d", i);
      unlink(pipeName);
   }
   for (i = 0; i < nreducers; i++)
   {
      sprintf(pipeName, "masterPipe%d", i);
      unlink(pipeName);
   }
   if (inter == 1)
   {
      deleteFiles(nmappers, "buff");
   }

   status = deleteFiles(nmappers, "split");
   if (status)
   {
      return status;
   }
   return 0;
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
   {
      allocator[i] = (int *)calloc(20, sizeof(int) * 20);
      if (allocator[i] == NULL)
      {
         perror("There was a problem allocating memory\n");
         return -1;
      }
   }
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