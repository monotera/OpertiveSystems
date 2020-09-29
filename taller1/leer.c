#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
   FILE *fp;
   printf("Programa que intenta abrir el archivo file.txt\n");
   /* Intentando abrir el archivo */
   fp = fopen(argv[1], "r");
   if( fp == NULL ) {
      perror("fopen: Could not open file for writing");
      return(-1);
   }
   fclose(fp);
      
   return(0);
}
/*El programa imprime la linea “Programa que intenta abrir 
el archivo file.txt” y termina su ejecucion sin dar errores
 ya que en el programa hay un condicional que indica en caso 
 de que el valor sea nulo retornar -1*/