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

void split(char *logfile, int lines, int nmappers); /*Create split files*/
void createMappers(int nmappers, char *command, map * mappers);
void mapper(char *split, char *command, map * mappers);
int createReducers(int nreducers, int nmappers);
int reducer(); /* creates output files */
void printAnswer(int total);

#endif