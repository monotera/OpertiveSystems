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

void split(char *logfile, int lines, int nmappers); /*Create split files*/
void createMappers(int nmappers, char *command);
void mapper(char *split, char *command);
void createBuf(map *mapper);
void createReducers(int nreducers, int nmappers);
void reducer(int intermediate); /* creates output files*/
void printAnswer(int nreducers);



#endif