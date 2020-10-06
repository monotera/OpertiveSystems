/**
* Name: Analogh.c (main)
* Operating Systems - First Release
* Authors: Carlos Andres Erazo Garzon
*          Nelson Alejandro Mosquera Barrera
*          Gabriel Andres Niño Carvajal
* Date: 4 / Oct / 2020
* Description: Main document for the implementation of threads, 
*              it corresponds to the interface to which the user 
*              has access together with  the calls to the necessary functions.
**/

#include "libs/libThreads.h"

#define ONE 1
#define ZERO 0
#define FIVE 5

/*
    compile file = gcc -pthread -std=c89  -o  analogh analogh.c libs/libThreads.c
*/

int main(int argc, char *argv[])
{
  /* if (argc != FIVE)
   {
      perror("Error : wrong number of parameters\n");
      exit(-1);
   }*/
   /*int lines = 0;
   int nmappers = 0;
   int nreducers = 0;
   int status;
   lines = atoi(argv[2]);
   nmappers = atoi(argv[3]);
   nreducers = atoi(argv[4]);
   status = validationParameters(argv[1], lines, nmappers, nreducers);
   if (status != 0)
   {
      printf("Good bye!");
      exit(-1);
   }*/
   while (ONE)
   {
      int option = 0;
      printf("\nChoose between option 1 or 2 using the keyboard\n");
      printf("1. Make a query\n");
      printf("2. log out\n");
      scanf("%d", &option);
      switch (option)
      {
      case 1:
         printf("Hello, please insert your command line (column,sign,value)\n");
         printf("\"Column\" must be a number\n");
         printf("\"Sign\" must be an operator between: \"=\", \"<\", \">\", \"<=\", \">= \"\n");
         printf("\"Value\" must be a number\n");
         printf("$");
         char *command;
         scanf("%s", command);
         int output = -1;
         /*output = processControl(argv[1], lines, nmappers, nreducers, command);*/
         output = processControl("log10000",10000,10,5,"5,<,1");
         if (output >= ZERO)
         {
            printf("there are %d records that meet the condition\n", output);
         }

         break;
      case 2:
         printf("Good bye world\n");
         exit(0);
         break;
      default:
         perror("Error: Invalid selection\n");
         printf("%s %d", "The registered selection was: \n", option);
         break;
      }
   }
}
