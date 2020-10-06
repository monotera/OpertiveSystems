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

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter)
{
   struct timeval start;
   struct timeval end;

   gettimeofday(&start, NULL);

   int status = 0;
   status = split(log, lines, nmappers);
   if (status)
   {
      return status;
   }
   struct command com;
   com = transform_command(command);
   if (com.dif == MONE)
   {
      perror("Error: invalid command\n");
      deleteFiles(nmappers, "split");
      return -1;
   }
   status = createMappers(nmappers, com);
   if (inter == ZERO)
   {
      status = deleteFiles(nmappers, "split");
      if (status)
      {
         return status;
      }
   }
   if (status)
   {
      deleteFiles(nmappers, "split");
      return status;
   }
   status = createReducers(nreducers, nmappers);
   if (inter == ZERO)
   {
      status = deleteFiles(nmappers, "buff");
      if (status)
      {
         return status;
      }
   }
   if (status)
   {
      return status;
   }
   if (status)
   {
      deleteFiles(nmappers, "split");
      return status;
   }
   status = printAnswer(nreducers);
   if (inter == ZERO)
   {
      deleteFiles(nreducers, "output");
   }
   if (status == MONE)
   {
      return status;
   }
   gettimeofday(&end, NULL);
   printf("Time of execution: %ld ms\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

   return status;
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
int createMappers(int nmappers, command command)
{
   int status = 0;
   int i = 0;
   int stat = 0;

   pid_t pidC;

   char *aux = (char *)malloc(11);
   strcpy(aux, "split0.txt");

   for (i = ZERO; i < nmappers; i++)
   {
      pidC = fork();
      if (pidC > ZERO)
      {
         continue;
      }
      else if (pidC == ZERO)
      {
         sprintf(aux, "split%d.txt", i);
         stat = mapper(aux, command, i);
         if (stat)
         {
            return -1;
         }
         exit(0);
      }
      else
      {
         perror("Fork: ");
         exit(-1);
      }
   }

   for (i = ZERO; i < nmappers; i++)
   {
      pidC = wait(&status);
      /*printf("P! padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidC);*/
      if (pidC == MONE)
      {
         stat = -1;
      }
   }
   return stat;
}
int mapper(char *split, command com, int iter)
{
   map x;

   char *buff = (char *)calloc(20, sizeof(char) * 20);
   sprintf(buff, "buff%d.txt", iter);
   FILE *writer = fopen(buff, "w");
   FILE *file = fopen(split, "r");
   if (file != NULL && writer != NULL)
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
            }
            break;
         case 2:
            if (buf[h] > com.eq)
            {
               x.value = buf[h];
            }
            break;
         case 3:
            if (buf[h] == com.eq)
            {
               x.value = buf[h];
            }
            break;
         case 4:
            if (buf[h] >= com.eq)
            {
               x.value = buf[h];
            }
            break;
         case 5:
            if (buf[h] <= com.eq)
            {
               x.value = buf[h];
            }
            break;
         default:
            break;
         }
         if (x.value != OTHER)
         {
            fprintf(writer, "%d %lf \n", x.key, x.value);
         }
      }
   }
   else
   {
      perror("Error: The file could not be open\n");
      return -1;
   }
   fclose(writer);
   fclose(file);
   return 0;
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

int clear(int nmappers, int nreducers)
{
   int status;
   status = deleteFiles(nmappers, "split");
   if (status)
   {
      return -1;
   }
   status = deleteFiles(nmappers, "buff");
   if (status)
   {
      return -1;
   }
   status = deleteFiles(nreducers, "output");
   if (status)
   {
      return -1;
   }
   return 0;
}
int printAnswer(int nreducers)
{
   int i = 0;
   char *out = (char *)calloc(20, sizeof(char) * 20);
   int sum = 0;
   int aux = 0;
   FILE *fp;
   for (i = ZERO; i < nreducers; i++)
   {
      sprintf(out, "output%d.txt", i);
      fp = fopen(out, "r");
      if (fp != NULL)
      {
         fscanf(fp, "%d", &aux);
         sum += aux;
      }
      else
      {
         perror("Error:");
         return -1;
      }
      fclose(fp);
   }
   return sum;
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
   if (col <= ZERO || col > NT) /*Erro message columns cant be negative an 0 means its a letter*/
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

int createReducers(int nreducers, int nmappers)
{
   int *assignments[nreducers];
   int i = 0;
   int k = 0;
   int j = 0;
   int z = 0;
   int y = 0;
   int stat = 0;
   int status = 0;
   pid_t pidE;

   for (i = ZERO; i < nreducers; i++)
      assignments[i] = (int *)calloc(20, sizeof(int) * 20);
   i = 0;
   while (i < nmappers)
   {
      assignments[j][k] = i;
      assignments[j][k + 1] = -1;
      i++;
      j++;
      if (j == nreducers)
      {
         j = 0;
         k++;
      }
   }

   for (z = ZERO; z < nreducers; z++)
   {
      pidE = fork();
      if (pidE > ZERO)
      {
         continue;
      }
      if (pidE == ZERO)
      {
         stat = reducer(assignments[z], z);
         if (stat)
         {
            return -1;
         }
         exit(0);
      }
      else
      {
         perror("Fork: ");
         exit(-1);
      }
   }

   for (y = ZERO; y < nreducers; y++)
   {
      pidE = wait(&status);
      if (status)
      {
         stat = -1;
      }

      /*printf("P2 padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidE);*/
   }
   return stat;
}

int reducer(int *assignments, int index)
{
   int i = 0;
   int suma = 0;
   char *buff;
   buff = (char *)calloc(20, sizeof(char));
   char *out;
   out = (char *)calloc(20, sizeof(char));
   sprintf(out, "output%d.txt", index);
   FILE *writer;
   FILE *fp;

   while (assignments[i] != MONE)
   {
      char aux;

      sprintf(buff, "buff%d.txt", assignments[i]);
      fp = fopen(buff, "r");
      if (fp != NULL)
      {
         aux = getc(fp);
         while (aux != EOF)
         {
            if (aux == '\n')
            {
               suma++;
            }
            aux = getc(fp);
         }
      }
      else
      {
         perror("Error: ");
      }

      i++;
      fclose(fp);
   }
   writer = fopen(out, "w");
   if (writer == NULL)
   {
      perror("Error: ");
      return -1;
   }
   fprintf(writer, "%d", suma);
   fclose(writer);
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