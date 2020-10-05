/**
 * Nombre: libProcess.h
 * Sistemas operativos: primera entrega
 * Autores: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Fecha: 4/oct/2020
 * Descripcion: 
 **/

#ifndef _LIBPROCESS_H_
#define _LIBPROCESS_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

typedef struct map
{
   int key;
   int value;
} map;

typedef struct command
{
   int col;
   int dif;
   int eq;
} command;

/**
 * Name: Process control.
 * Inputs: Nombre del documento, Cantidad de lineas del documento, Cantidad de mappers, Cantidad de reducers, Comando de busqueda.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Funcion que gestiona el correcto funcionamiento del programa, asimismo registra el tiempo que demora la ejecucion.
 * */

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter);

/**
 * Name: Split.
 * Inputs: Nombre del documento, Cantidad de lineas del documento, Cantidad de mappers.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Divide el documento principal segun la cantidad de mappers, esto se guarda en archivos de nombre split con el respectivo numero consecutivo de su division.
 * */

int split(char *logfile, int lines, int nmappers); /*Create split files*/

/**
 * Name: Create mappers.
 * Inputs: Cantidad de mappers, Comando de busqueda, Entero que determina si se borran o no los archivos durante la ejecución.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Crea los mappers y los asigna al fragmento del documento.
 * */

int createMappers(int nmappers, command com, int intermediate);

/**
 * Name: Mapper.
 * Inputs: Nombre del documento, Comando de busqueda, Entero que corresponde al split asignado a un mapper, Entero que determina si se borran o no los archivos durante la ejecución.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: El mapper comienza la busqueda de aciertos con respecto al comando establecido, esto se guarda en un nuevo archivo de nombre buf con el respectivo numero del mapper.
 * */

int mapper(char *split, command com, int iter, int intermediate);

/**
 * Name: Transform command.
 * Inputs: Comando de busqueda.
 * Outputs: Comando de busqueda con el que se pueden realizar operaciones.
 * Description: A partir de la entrada del usuario que indica los parametros a buscar en los registros, la funcion reconoce cada parte del comando de busqueda.
 * */

struct command transform_command(char *command);

/**
 * Name: Validate command.
 * Inputs: Columna del documento, Signo o condicional de la busqueda, Parametro, Bandera.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Se validan los campos del comando de busqueda.
 * */

int validate_command(int col, char *dif, int eq, int flag);

/**
 * Name: Create reducers.
 * Inputs: Numero de reducers, Numero de mappers.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Crea los reducers y los asigna al archivo buff correspondiente.
 * */

int createReducers(int nreducers, int nmappers);

/**
 * Name: Reducer.
 * Inputs: Entero que determina si se borran o no los archivos durante la ejecución, Entero que determina que archivo buff se le fue asignado, Entero que determina donde se guarda el resultado.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Se cuentan el numero de aciertos de los documentos buff asignados y se guardan los resultados en un documento output con el numero consecutivo correspondiente.
 * */

int reducer(int intermediate, int *assigments, int index); /* creates output files*/

/**
 * Name: Print answer.
 * Inputs: Numero de reducers.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Finalmente se leen los archivos output creados y se da el resultado.
 * */

int printAnswer(int nreducers);

/**
 * Name: Clear
 * Inputs: Ninguna
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Dado que se puede dar el caso de que no se borren los archivos mientras se ejecuta el programa, la funcion se encarga de borrar los archivos despues de la ejecución principal.
 * */

int clear();

#endif