/*Nelson Mosquera Carlos Erazo Gabriel Niño*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "empleados.h"

int bandera = 1;

void EnviarPipe(emple *miemp, int cuantos, char pipe[])
{

  int i, creado, fd;
  char message[100];

  int messagelen;
  do
  {
    fd = open(pipe, O_WRONLY | O_NONBLOCK);
    if (fd == -1)
      sleep(1);
  } while (fd == -1);

  for (i = 0; i < cuantos; i++)
  {
    sprintf(message, "%s %d %f %d\n", miemp[i].nombre, miemp[i].edad, miemp[i].salario, getpid());
    messagelen = strlen(message);
    write(fd, message, messagelen);
    printf("Sending throught the pipe \n");
    sleep(1);
  }
}

/* asignar
   inserta un nuevo empleado en la lista en la posicion pos
   e inicializa su edad, tiempo de trabajo y nombre
*/
void asignar(emple *emp, char *nombre, int edad, float salary, int pos)
{

  char *p;

  emp[pos].edad = edad;
  emp[pos].salario = salary;
  strcpy(emp[pos].nombre, nombre);
}

/* imprimir
   imprime los primeros cuantos empleados del arreglo
*/
void imprimir(emple *emp, int cuantos)
{
  int i;

  printf("Empleados:\n");

  for (i = 0; i < cuantos; i++)
  {
    printf("%s %d %f\n", emp[i].nombre, emp[i].edad, emp[i].salario);
  }
}
void signalHandler()
{
  printf("Programa buscador.c esta terminando, PID: %d\n", getpid());
  bandera = 0;
}
int main(int argc, char *argv[])
{
  emple miemp[MAXEMP];
  char line[MAXLIN], nom[MAXNOMBRE];
  FILE *fp;
  int i, edad = 0;
  float salario;
  signal(SIGUSR1, signalHandler);
  fp = fopen(argv[1], "r");
  i = 0;

  while (!feof(fp))
  {
    fscanf(fp, "%s %d %f\n", nom, &edad, &salario);
    asignar(miemp, nom, edad, salario, i++);
  }
  fclose(fp);
  /*imprimir(miemp, i);*/

  while (bandera)
  {
    EnviarPipe(miemp, i, argv[2]);
  }
}
