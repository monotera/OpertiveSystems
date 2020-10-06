/**
 * Name: libProcess.h
 * Operative systems first project
 * Authors: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Date: 4/oct/2020
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
   double value;
} map;

typedef struct command
{
   int col;
   int dif;
   double eq;
} command;

/**
 * Name: Process control.
 * Inputs: Name of the document, Number of lines in the document, Number of mappers, Number of reducers, Search command.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Function that manages the correct operation of the program, also records the time it takes to execute.
 * */

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter);

/**
 * Name: Split.
 * Inputs: Name of the document, Number of lines in the document, Number of mappers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Divides the main document according to the number of mappers, this is saved in files named split with the respective consecutive number of its division.
 * */

int split(char *logfile, int lines, int nmappers); 

/**
 * Name: Create mappers.
 * Inputs: Number of mappers, Search command
 * Outputs: Integer that verifies the operation of the function.
 * Description: Creates the mappers and assigns them to the document fragment.
 * */

int createMappers(int nmappers, command com);

/**
 * Name: Mapper.
 * Inputs: Document name, Search command, Integer that corresponds to the split assigned to a mapper
 * Outputs: Integer that verifies the operation of the function.
 * Description: The mapper begins the search for matches according to the established command, this is saved in a new file named buf with the respective mapper number.
 * */

int mapper(char *split, command com, int iter);

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
 * Name: Create reducers.
 * Inputs: Number of reducers, Number of mappers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Creates the reducers and assigns them to the corresponding buff file.
 * */

int createReducers(int nreducers, int nmappers);

/**
 * Name: Reducer.
 * Inputs: Integer that determines which buff file was assigned to it, Integer that determines where the result is saved.
 * Outputs: Integer that verifies the operation of the function.
 * Description: The number of hits of the assigned buff documents are counted and the results are saved in an output document with the corresponding consecutive number.
 * */

int reducer(int *assigments, int index); /* creates output files*/

/**
 * Name: Print answer.
 * Inputs: Number of reducers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Finally the output files created are read and the result is given.
 * */

int printAnswer(int nreducers);

/**
 * Name: Clear
 * Inputs: number of mappers and number of reducers
 * Outputs: Integer that verifies the operation of the function.
 * Description: Since it may happen that the files are not deleted while the program is running, the function takes care of deleting the files after the main execution.
 * */

int clear(int nmappers, int nreducers);

/**
 * Name: Line counter.
 * Inputs: File name.
 * Outputs: Number of lines that the file has.
 * Description: Counts the number of lines in the file.
 * */

int lineCounter(char *log);

/**
 * Name: validationParameters.
 * Inputs: File name, number of lines entered by parameters, number of mappers, number of reducers, intermediate.
 * Outputs: 0 if the parameters are valid, -1 if they are invalid.
 * Description: Validates number of parameters.
 * */

int validationParameters(char *log, int lines, int nmappers, int nreducers, int inter);

#endif