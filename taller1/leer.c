#include <stdio.h>
#include <stdlib.h>

int main ()
{
   FILE *fp;
   printf("Programa que intenta abrir el archivo file.txt\n");
   /* Intentando abrir el archivo */
   fp = fopen("nope.txt", "r");
   if( fp == NULL ) {
      perror("fopen: Could not open file for writing");
      return(-1);
   }
   fclose(fp);
      
   return(0);
}
