#ifndef _LIBPROCESS_H_
#define _LIBPROCESS_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

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
void split(char *logfile, int lines, int nmappers); /*Create split files*/
void createMappers(int nmappers, char *command, int intermediate);
void mapper(char *split, char *command, int iter, int intermediate);
void createBuf(map *mapper);
void createReducers(int nreducers, int nmappers);
int reducer(int intermediate,int * assigments, int index); /* creates output files*/
int printAnswer(int nreducers);
void clear();


#endif