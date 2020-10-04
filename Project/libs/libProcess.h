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

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter);
int split(char *logfile, int lines, int nmappers); /*Create split files*/
int createMappers(int nmappers, command com, int intermediate);
int mapper(char *split, command com, int iter, int intermediate);
struct command transform_command(char *command);
int validate_command(int col, char *dif, int eq, int flag);
void createBuf(map *mapper);
int createReducers(int nreducers, int nmappers);
int reducer(int intermediate,int * assigments, int index); /* creates output files*/
int printAnswer(int nreducers);
int clear();


#endif