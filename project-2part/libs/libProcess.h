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
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int init(int *pIdM, int *pIdR, int nmappers, int nreducers);
void signalHandlerFinisher();
/**
 * Name: Process control.
 * Inputs: Name of the document, Number of lines in the document, Number of mappers, Number of reducers, Search command.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Function that manages the correct operation of the program, also records the time it takes to execute.
 * */

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int *pIdM, int *pIdR);

/**
 * Name: Split.
 * Inputs: Name of the document, Number of lines in the document, Number of mappers.
 * Outputs: Integer that verifies the operation of the function.
 * Description: Divides the main document according to the number of mappers, this is saved in files named split with the respective consecutive number of its division.
 * */

int split(char *logfile, int lines, int nmappers);

/**
 * Name: validationParameters.
 * Inputs: File name, number of lines entered by parameters, number of mappers, number of reducers, intermediate.
 * Outputs: 0 if the parameters are valid, -1 if they are invalid.
 * Description: Validates number of parameters.
 * */

int validationParameters(char *log, int lines, int nmappers, int nreducers);

/**
 * Name: Line counter.
 * Inputs: File name.
 * Outputs: Number of lines that the file has.
 * Description: Counts the number of lines in the file.
 * */

int lineCounter(char *log);

int sendCommand(char *command, int nmappers, int *pIdM);

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

int deleteFiles(int canti, char *type);

int finalizer(int *pIdM, int nmappers);

int assignPipes(int nmappers,int nreducers, int **allocator);

int mapper(int id,int redId);
int reduccer(int id, int *allocator);
#endif