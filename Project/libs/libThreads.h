#ifndef _LIBTHREADS_H_
#define _LIBTHREADS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

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
typedef struct parameters
{
    char *split;
    char *command;
    int status;
} parameters;

int processControl(char *log, int lines, int nmappers, int nreducers, char *command);
void intializer(int nmappers);
int split(char *logfile, int lines, int nmappers); /*Create split files*/
int deleteSplit(int nmappers);
int createMappers(int nmappers, char *command);
struct command transform_command(char *command);
int validate_command(int col, char *dif, int eq, int flag);
void *mapper(void *info);
struct map line_checker(char *str, int col, int dif, int eq);
int createReducers(int nreducers, int nmappers);
void *reducer(void *assignments); /* creates output files */
void printAnswer(int total);
int lineCounter(char *log);
void clear(int nmappers);

#endif