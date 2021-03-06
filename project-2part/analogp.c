/**
* Name: Analogp.c (main)
* Operating Systems - Second Release
* Authors: Carlos Andres Erazo Garzon
*          Nelson Alejandro Mosquera Barrera
*          Gabriel Andres Niño Carvajal
* Date: 19 / Nov / 2020
* Description: Main document for the implementation of process and pipes, 
*              it corresponds to the interface to which the user 
*              has access together with  the calls to the necessary functions.
**/
#include "libs/libProcess.h"

#define ONE 1
#define ZERO 0
#define SIX 6

/*
     compile file = gcc -std=c89 -o analogp analogp.c libs/libProcess.c
*/

int main(int argc, char *argv[])
{
   if (argc != SIX)
   {
      perror("Error : wrong number of parameters\n");
      printf("it should be: analogp  logfile  lineas  nmappers nreducers intermedios ");
      exit(-1);
   }
   int lines = 0;
   int nmappers = 0;
   int nreducers = 0;
   int inter = 0;
   int status = -1;
   int *pIdM;
   int *pIdR;

   lines = atoi(argv[2]);
   nmappers = atoi(argv[3]);
   nreducers = atoi(argv[4]);
   inter = atoi(argv[5]);
   status = validationParameters(argv[1], lines, nmappers, nreducers, inter);
   if (status != 0)
   {
      printf("\nError: problem wih parameters\n");
      printf("it should be: analogp  logfile  lineas  nmappers nreducers intermedios ");
      exit(-1);
   }
   pIdM = (int *)calloc(nmappers, sizeof(int));
   if (pIdM == NULL)
   {
      perror("There was a problem allocating memory\n");
      return -1;
   }
   pIdR = (int *)calloc(nreducers, sizeof(int));
   if (pIdR == NULL)
   {
      perror("There was a problem allocating memory\n");
      return -1;
   }
   status = init(pIdM, pIdR, nmappers, nreducers, argv[1], lines, inter);
   if (status != 0)
   {
      exit(-1);
   }
   while (ONE)
   {
      int option = 0;
      printf("Choose between option 1 or 2 using the keyboard\n");
      printf("1. Make a query\n");
      printf("2. log out\n");
      scanf("%d", &option);
      switch (option)
      {
      case 1:
         if (inter == ONE)
         {
            printf("\nHello!, Checking for files deletions!\n");
            deleteFiles(nmappers, "buff");
            printf("Done!\n");
         }
         printf("Please insert your command line (column,sign,value)\n");
         printf("\"Column\" must be a number\n");
         printf("\"Sign\" must be an operator between: \"=\", \"<\", \">\", \"<=\", \">= \"\n");
         printf("\"Value\" must be a number\n");
         printf("$");
         char *command;
         scanf("%s", command);

         int output = -1;
         output = processControl(nmappers, nreducers, command, pIdM, pIdR);
         if (output >= ZERO)
         {
            printf("there are %d records that meet the condition\n", output);
         }
         break;
      case 2:
         printf("Good bye world\n");
         int i;
         status = finalizer(nmappers, nreducers, inter);
         if (status != ZERO)
         {
            perror("There was a problem deleting the files\n");
         }
         for (i = 0; i < nmappers; i++)
         {
            printf("Mapper %d with pId %d is ending...\n", i, pIdM[i]);
            kill(pIdM[i], SIGCONT);
            kill(pIdM[i], SIGUSR1);
         }
         for (i = 0; i < nreducers; i++)
         {
            printf("Reducer %d with pId %d is ending...\n", i, pIdR[i]);
            kill(pIdR[i], SIGCONT);
            kill(pIdR[i], SIGUSR1);
         }
         wait(&status);
         free(pIdM);
         free(pIdR);
         exit(0);
         break;
      default:
         perror("Error: Invalid selection\n");
         printf("%s %d", "The registered selection was: ", option);
         break;
      }
   }
}
