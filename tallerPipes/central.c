#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "empleados.h"

void separar(char *palabra, char mat[3][100])
{
  int i = 0;
  char *token = strtok(palabra, " ");
  while (token != NULL)
  {
    strcpy(mat[i], token);
    token = strtok(NULL, " ");
    i++;
  }
}

int main(int argc, char **argv)
{
  int fd, pid, n, bytes, creado;
  emple em;

  mode_t fifo_mode = S_IRUSR | S_IWUSR;
  unlink(argv[1]); // por si ya existe

  if (mkfifo(argv[1], fifo_mode) == -1)
  {
    perror("mkfifo");
    exit(1);
  }

  // De esta forma se puede iniciar cualquiera de los dos procesos
  // primero y funciona.
  do
  {
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
      perror("pipe");
      printf(" Se volvera a intentar despues\n");
      sleep(5);
    }
    else
      creado = 1;
  } while (creado == 0);

  printf("Abrio el pipe\n");
  char message[100];
  char mat[4][100];
  int i = 0;
  int edad, id;
  while (read(fd, message, 100) > 0)
  {
    separar(message, mat);
    edad = atoi(mat[1]);
    id = atoi(mat[3]);
    if (edad != 0)
    {
      if (edad >= 21 && edad <= 50)
      {
        printf("Nombre: %s ", mat[0]);
        printf("Salario: %s\n", mat[1]);
      }
    }
    else
    {
      printf("buscador.c %d fin del procesamiento\n", id);
      kill(id,SIGUSR1);
      sleep(0.5);
      kill(id,9);
    }
  }
}