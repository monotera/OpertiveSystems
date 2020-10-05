/**
 * Nombre: libThreads.c
 * Sistemas operativos: primera entrega
 * Autores: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Fecha: 4/oct/2020
 * Descripcion: Implementación de la libreria "libThreads.h".
 **/

#include "libThreads.h"

#define CHUNK 1024
#define ONE 1
#define ZERO 0
#define MONE -1
#define NT 18

int cont = 0;

int processControl(char *log, int lines, int nmappers, int nreducers, char *command)
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
   map *mapper = calloc(nmappers, sizeof(map));
   struct command com;
   com = transform_command(command);
   if (com.dif == MONE)
   {
      perror("Invalid command\n");
      deleteSplit(nmappers);
      return -1;
   }
   status = createMappers(nmappers, com, mapper);
   if (status)
   {
      return status;
   }
   status = deleteSplit(nmappers);
   if (status)
   {
      return status;
   }
   status = createReducers(nreducers, nmappers, mapper);
   if (status == MONE)
   {
      return status;
   }

   gettimeofday(&end, NULL);
   printf("Time of execution: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
   printf("ms");
   return status;
}

int split(char *logfile, int lines, int nmappers)
{

   FILE *file = fopen(logfile, "r");
   int sub_lines;
   sub_lines = ((lines) / (nmappers));

   if (file != NULL && sub_lines > ZERO)
   {

      char *str;
      str = (char *)malloc(CHUNK);
      int cont_lines = 1;
      int cont_splitFer = 0;
      char *aux;
      aux = (char *)malloc(11);
      strcpy(aux, "split0.txt");
      FILE *writer;
      int flag = 1;

      while (fgets(str, CHUNK, file))
      {
         if (flag == ONE)
         {
            writer = fopen(aux, "w");
            if (writer == NULL)
            {
               perror("Error: ");
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
      perror("Error: ");
      return -1;
   }
   return 0;
}

int createMappers(int nmappers, command commandM, map *mapperStruct)
{
   int rc = 0;
   int rj = 0;
   pthread_t thread1[nmappers];
   map m[nmappers];
   int i = 0;
   void *status;
   parameters par[nmappers];

   for (i = ZERO; i < nmappers; i++)
   {
      par[i].split = (char *)calloc(CHUNK, sizeof(char) * CHUNK);
      sprintf(par[i].split, "split%d.txt", i);
      par[i].com = commandM;
      par[i].numLines = lineCounter(par[i].split);
      par[i].mapper.key = calloc(par[i].numLines + 1, sizeof(int));
      par[i].mapper.value = calloc(par[i].numLines + 1, sizeof(int));
   }

   int j = 0;
   for (j = ZERO; j < nmappers; j++)
   {
      rc = pthread_create(&thread1[j], NULL, mapper, (void *)&par[j]);
      if (rc)
      {
         perror("Error: ");
         return -1;
      }
      if (par[i].numLines == MONE)
      {
         return -1;
      }
   }
   int k = 0;
   for (k = ZERO; k < nmappers; k++)
   {
      rj = pthread_join(thread1[k], NULL);
      if (rj)
      {
         perror("Error: ");
         return -1;
      }
   }
   for (i = ZERO; i < nmappers; i++)
   {
      mapperStruct[i] = par[i].mapper;
   }
   return 0;
}

void *mapper(void *infor)
{
   struct parameters *info;
   info = (parameters *)infor;
   char *splitFile;
   splitFile = info->split;
   info->status = 0;

   FILE *file = fopen(splitFile, "r");

   int i = 1;
   map *aux_mapper = (map *)calloc(info->numLines, sizeof(map));
   if (file != NULL)
   {
      char *str = (char *)malloc(CHUNK);
      int h = 0;
      h = info->com.col;
      int buf[19];
      int z = 1;
      while (fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                    &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6],
                    &buf[7], &buf[8], &buf[9], &buf[10], &buf[11], &buf[12],
                    &buf[13], &buf[14], &buf[15], &buf[16], &buf[17], &buf[18]) != EOF)
      {

         switch (info->com.dif)
         {
         case 1:
            if (buf[h] < info->com.eq)
            {
               info->mapper.key[z] = buf[1];
               info->mapper.value[z] = buf[h];
               z++;
            }
            break;
         case 2:
            if (buf[h] > info->com.eq)
            {
               info->mapper.key[z] = buf[1];
               info->mapper.value[z] = buf[h];
               z++;
            }
            break;
         case 3:
            if (buf[h] == info->com.eq)
            {
               info->mapper.key[z] = buf[1];
               info->mapper.value[z] = buf[h];
               z++;
            }
            break;
         case 4:
            if (buf[h] >= info->com.eq)
            {
               info->mapper.key[z] = buf[1];
               info->mapper.value[z] = buf[h];
               z++;
            }

            break;
         case 5:
            if (buf[h] <= info->com.eq)
            {
               info->mapper.key[z] = buf[1];
               info->mapper.value[z] = buf[h];
               z++;
            }
            break;

         default:
            break;
         }
      }
      info->mapper.key[0] = z;
      fclose(file);
   }
   else
   {
      perror("Error: The file could not be open\n");
      info->status = -1;
   }

   pthread_exit(NULL);
}
int createReducers(int nreducers, int nmappers, map *mapper)
{

   pr par[nreducers];
   pthread_t thread1[nreducers];
   int *assignments[nreducers];

   int i = 0;
   int k = 0;
   int j = 0;
   int rc = 0;
   int rj = 0;

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

   int suma = 0;
   for (i = ZERO; i < nreducers; i++)
   {
      par[i].assigments = assignments[i];
      par[i].mapper = mapper;
      par[i].res = 0;
      rc = pthread_create(&thread1[i], NULL, reducer, (void *)&par[i]);
      if (rc)
      {
         perror("Error: ");
         return -1;
      }
   }
   for (i = ZERO; i < nreducers; i++)
   {
      rj = pthread_join(thread1[i], NULL);
      if (rj)
      {
         perror("Error: ");
         return -1;
      }
   }
   for (i = ZERO; i < nreducers; i++)
   {
      suma += par[i].res;
   }
   return suma;
}

void *reducer(void *assig)
{
   struct pr *info;
   info = (pr *)assig;
   int i = 0;
   map *mapper = info->mapper;
   int j = 0;
   while (info->assigments[i] != MONE)
   {
      int indej = info->assigments[i];
      info->res += mapper[indej].key[0] - 1;
      i++;
   }
}

int deleteSplit(int nmappers)
{
   int i;
   char *aux = (char *)malloc(11);
   strcpy(aux, "split0.txt");
   for (i = ZERO; i < nmappers; i++)
   {
      int rc = 0;
      rc = remove(aux);
      if (rc)
      {
         perror("The file couldnt be delete it");
         free(aux);
         return -1;
      }
      sprintf(aux, "split%d.txt", i + 1);
   }
   free(aux);
   return 0;
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
   char *dif = (char *)malloc(3);
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
            flag = 1;
         eq = atoi(token);
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
