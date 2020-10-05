/**
 * Nombre: Analogh.c (main)
 * Sistemas operativos: primera entrega
 * Autores: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Fecha: 4/oct/2020
 * Descripcion: Documento principal para la implementacion de hilos, corresponde
 *              a la interfaz a la que tiene acceso el ususario junto con los 
 *              llamados a las funciones necesarias.
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
   if (argc != FIVE)
   {
      perror("Error : wrong number of parameters\n");
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
         printf("Hello, please insert your command line (column,sign,value)\n");
         printf("\"Column\" must be a number\n");
         printf("\"Sign\" must be an operator between: \"=\", \"<\", \">\", \"<=\", \">= \"\n");
         printf("\"Value\" must be a number\n");
         printf("$");
         char *command;
         int lines = 0;
         int nmappers = 0;
         int nreducers = 0;
         scanf("%s", command);
         lines = atoi(argv[2]);
         nmappers = atoi(argv[3]);
         nreducers = atoi(argv[4]);
         if (nreducers == ZERO)
         {
            perror("Invalid number of reducers");
         }
         else
         {
            int output = -1;
            output = processControl(argv[1], lines, nmappers, nreducers, command);
            if (output >= ZERO)
            {
               printf("there are %d records that meet the condition\n", output);
            }
         }
         break;
      case 2:
         printf("Good bye world\n");
         exit(0);
         break;
      default:
         perror("Error: Invalid selection\n");
         printf("%s %d", "The registered selection was: ", option);
         break;
      }
   }
}
