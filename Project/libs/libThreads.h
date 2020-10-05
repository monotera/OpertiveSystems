/**
 * Nombre: libProcess.h
 * Sistemas operativos: primera entrega
 * Autores: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Fecha: 4/oct/2020
 * Descripcion: Funciones que corresponden a la implementación con procesos.
 **/

#ifndef _LIBTHREADS_H_
#define _LIBTHREADS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

typedef struct map
{
   int *key;
   int *value;
} map;
typedef struct command
{
   int col;
   int dif;
   int eq;
} command;
typedef struct parameters
{
   char *split;
   int status;
   command com;
   int numLines;
   map mapper;
} parameters;
typedef struct pr
{
   int *assigments;
   map *mapper;
   int res;
} pr;

/**
 * Name: processControl,
 * Inputs: Nombre del documento, Cantidad de lineas del documento, Cantidad de mappers, Cantidad de reducers, Comando de busqueda.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Funcion que gestiona el correcto funcionamiento del programa, asimismo registra el tiempo que demora la ejecucion.
 * */

int processControl(char *log, int lines, int nmappers, int nreducers, char *command);

/**
 * Name: split.
 * Inputs: Nombre del documento, Cantidad de lineas del documento, Cantidad de mappers.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Divide el documento principal segun la cantidad de mappers.
 * */

int split(char *logfile, int lines, int nmappers); /*Create split files*/

/**
 * Name: Delete split.
 * Inputs: Numero de mappers.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Se borran los archivos creados por la funcion split.
 * */

int deleteSplit(int nmappers);

/**
 * Name: CreateMappers
 * Inputs: Cantidad de mappers, Comando de busqueda, Mapa para realizar la busqueda.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Crea los mappers y los asigna al fragmento del documento.
 * */

int createMappers(int nmappers, command commandM, map *mapperStruct);

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
 * Name: Mapper.
 * Inputs: Informacion necesaria para el tratamiento de los datos.
 * Outputs: Ninguna
 * Description: El mapper comienza la busqueda de aciertos con respecto al comando establecido.
 * */

void *mapper(void *info);

/**
 * Name: Create reducers.
 * Inputs: Numero de reducers, Numero de mappers, mapa de resultado.
 * Outputs: Entero que verifica el funcionamiento de la función.
 * Description: Crea los reducers y los asigna al archivo buff correspondiente.
 * */

int createReducers(int nreducers, int nmappers, map *mapper);

/**
 * Name: Reducer.
 * Inputs: Información necesara para el tratamiento de los datos.
 * Outputs: Ninguno.
 * Description: Se cuentan el numero de aciertos de los documentos buff asignados.
 * */

void *reducer(void *assignments); /* creates output files */

/**
 * Name: Line counter.
 * Inputs: Nombre del archivo.
 * Outputs: Numero de lineas que tiene el archivo.
 * Description: Cuenta el numero de lineas del archivo.
 * */

int lineCounter(char *log);

#endif
