/**
 * Nombre: Analogp.c (main)
 * Sistemas operativos: primera entrega
 * Autores: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Fecha: 4/oct/2020
 * Descripcion: Documento principal para la implementacion de procesos, corresponde
 *              a la interfaz a la que tiene acceso el ususario junto con los 
 *              llamados a las funciones necesarias.
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
         printf("Hello!, Checking for files deletions!\n");
         int lines = 0;
         int nmappers = 0;
         int nreducers = 0;
         int inter = 0;
         lines = atoi(argv[2]);
         nmappers = atoi(argv[3]);
         nreducers = atoi(argv[4]);
         inter = atoi(argv[5]);
         if (inter == ONE)
         {
            clear(nmappers, nreducers);
         }
         printf("Done!, please insert your command line (column,sign,value)\n");
         printf("\"Column\" must be a number\n");
         printf("\"Sign\" must be an operator between: \"=\", \"<\", \">\", \"<=\", \">= \"\n");
         printf("\"Value\" must be a number\n");
         printf("$");
         char *command;
         scanf("%s", command);

         if (nreducers == ZERO)
         {
            perror("Invalid number of reducers");
         }
         if (inter > ONE || inter < ZERO)
         {
            perror("Error: inter must be 0 or 1");
            exit - 1;
         }
         else
         {

            int output = -1;
            output = processControl(argv[1], lines, nmappers, nreducers, command, inter);
            /*output = processControl("test1.txt", 534, 10, 3, "5,>=,1", 0);*/
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
