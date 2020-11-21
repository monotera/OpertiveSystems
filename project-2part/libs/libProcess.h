/**
 * Name: libProcess.h
 * Operative systems second project
 * Authors: Carlos Andres Erazo Garzon
 *          Nelson Alejandro Mosquera Barrera
 *          Gabriel Andres Niño Carvajal
 * Date: 19/nov/2020
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

/**
 * Name: signalHandlerMapper
 * Inputs: None
 * Outputs: None
 * Description: Function that manages SIGUSR1 signals for ending the mappers process
 * */
void signalHandlerMapper();

/**
 * Name: signalHandlerMapper
 * Inputs: None
 * Outputs: None
 * Description: Function that manages SIGUSR2 signals for ending the reducers process
 * */
void signalHandlerReducer();

/**
 * Name: init
 * Inputs: array of mappers pId, array of reducers pId, number of mappers, number of reducers, name of the file, number of lines, intermediary 
 * Outputs: integer for validation
 * Description: this function initializes every pipe and every process
 * */
int init(int *pIdM, int *pIdR, int nmappers, int nreducers, char *log, int lines, int inter);

/**
 * Name: Process control.
 * Inputs: Number of mappers, Number of reducers, Search command, array of mappers pId, array of reducers pId.
 * Outputs: integer for validation
 * Description: Function that manages the correct operation of the program, also records the time it takes to execute.
 * */

int processControl(int nmappers, int nreducers, char *command, int *pIdM, int *pIdR);

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

int validationParameters(char *log, int lines, int nmappers, int nreducers, int inter);

/**
 * Name: Line counter.
 * Inputs: File name.
 * Outputs: Number of lines that the file has.
 * Description: Counts the number of lines in the file.
 * */

int lineCounter(char *log);

/**
 * Name: sendCommand.
 * Inputs: users command, number of mappers, array of mappers pId.
 * Outputs: 0 if the parameters are valid, -1 if they are invalid.
 * Description: Sends the users command to the mappers.
 * */

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

/**
 * Name: deleteFiles.
 * Inputs: number of files, first part of the name of the file.
 * Outputs: -1 if the files couldn't be delete it
 * Description: function that deletes files.
 * */
int deleteFiles(int canti, char *type);

/**
 * Name: finalizer.
 * Inputs: Number of mappers, number of reducers.
 * Outputs: -1 if the files couldn't be delete it
 * Description: function that deletes files and unlinks pipes.
 * */
int finalizer(int nmappers, int nreducers, int inter);

/**
 * Name: assignPipes.
 * Inputs: Number of mappers, number of reducers, array of array that will store the schedule of the reducers.
 * Outputs: -1 if something went wrong
 * Description: function that assings pipes to the reducers.
 * */
int assignPipes(int nmappers, int nreducers, int **allocator);

/**
 * Name: Mapper.
 * Inputs: id of the mapper, id of the reducer, Integer that determines if buff files will be create it
 * Outputs: Integer that verifies the operation of the function.
 * Description: The mapper begins the search for matches according to the established command, then this is send to the reducer throug a pipe.
 * */
int mapper(int id, int redId, int inter);

/**
 * Name: Reducer.
 * Inputs: id of the reducer, Integer that determines where the result is saved.
 * Outputs: Integer that verifies the operation of the function.
 * Description: The number of hits of the assigned buff documents are counted and the results are send through a pipe.
 * */
int reducer(int id, int *abcd);

/**
 * Name: findMatch.
 * Inputs: file name, user's command, mapper id, mapper
 * Outputs: Integer that verifies the operation of the function.
 * Description: The mapper begins the search for matches according to the established command, and stores it on the mapper.
 * */
int findMatch(char *split, command com, int iter, map *maps);

#endif