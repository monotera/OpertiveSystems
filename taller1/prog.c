#include <stdio.h>
int main(int argc, char **argv)
{
  /* programa que imprime los argumentos de la l�nea de comandos */
  int i;
  long canti = strtol(argv[2], NULL, 10);
  for (i = 0; i < canti; ++i)
    printf("%s\n",argv[1]);
}
