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

int processControl(char *log, int lines, int nmappers, int nreducers, char *command);
int split(char *logfile, int lines, int nmappers); /*Create split files*/
int deleteSplit(int nmappers);
int createMappers(int nmappers, command commandM, map * mapperStruct);
struct command transform_command(char *command);
int validate_command(int col, char *dif, int eq, int flag);
void *mapper(void *info);
int createReducers(int nreducers, int nmappers, map *mapper);
void *reducer(void *assignments); /* creates output files */
int lineCounter(char *log);

#endif