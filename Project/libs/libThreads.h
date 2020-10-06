/**
 * Name: libThreads.h
 * Operative systems first project
 * Authors: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Date: 4/oct/2020
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
   float *key;
   double *value;
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
 * Inputs: Name of the document, Number of lines in the document, Number of mappers, Number of reducers, Search command.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Function that manages the correct operation of the program, also records the time it takes to execute.
 * */

int processControl(char *log, int lines, int nmappers, int nreducers, char *command);

/**
 * Name: split.
 * Inputs: Name of the document, Number of lines in the document, Number of mappers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Divides the main document according to the number of mappers.
 * */

int split(char *logfile, int lines, int nmappers); /*Create split files*/

/**
 * Name: Delete split.
 * Inputs: Number of mappers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: The files created by the split function are deleted.
 * */

int deleteSplit(int nmappers);

/**
 * Name: CreateMappers
 * Inputs: Number of mappers, Search command, Map to perform the search.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Creates the mappers and assigns them to the document fragment.
 * */

int createMappers(int nmappers, command commandM, map *mapperStruct);

/**
 * Name: Transform command.
 * Inputs: Search command.
 * Outputs: Search command with which operations can be performed.
 * Description: From the user input that indicates the parameters to be searched in the registers, the function recognizes each part of the search command.
 * */

struct command transform_command(char *command);

/**
 * Name: Validate command.
 * Inputs: Document column, Search sign or conditional, Parameter, Flag.
 * Outputs: Integer that verifies the operation of the function.
 * Description: The fields of the search command are validated.
 * */

int validate_command(int col, char *dif, int eq, int flag);

/**
 * Name: Mapper.
 * Inputs: Information necessary for data processing.
 * Outputs: None
 * Description: The mapper begins the search for matches according to the established command, saving them in the array of mappers.
 * */

void *mapper(void *info);

/**
 * Name: Create reducers.
 * Inputs: Number of reducers, Number of mappers, result map.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Creates the reducers and assigns them to the corresponding buff file.
 * */

int createReducers(int nreducers, int nmappers, map *mapper);

/**
 * Name: Reducer.
 * Inputs: Information necessary for data processing.
 * Outputs: None.
 * Description: The number of hits of the assigned buff documents are counted.
 * */

void *reducer(void *assignments); /* creates output files */

/**
 * Name: Line counter.
 * Inputs: File name.
 * Outputs: Number of lines that the file has.
 * Description: Counts the number of lines in the file.
 * */

int lineCounter(char *log);

/**
 * Name: validationParameters.
 * Inputs: File name, number of lines entered by parameters, number of mappers, number of reducers.
 * Outputs: 0 if the parameters are valid, -1 if they are invalid.
 * Description: Validates number of parameters.
 * */
int validationParameters(char *log, int lines, int nmappers, int nreducers);

#endif
