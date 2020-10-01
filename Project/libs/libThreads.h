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

void split(char *logfile, int lines, int nmappers); /*Create split files*/
void createMappers(int nmappers, char *command);
struct command transform_command(char *command);
int validate_command(int col, char *dif, int eq);
void mapper(char *split, int col, int dif, int eq);
struct map line_checker(char *str, int col, int dif, int eq);
int createReducers(int nreducers, int nmappers);
int reducer(); /* creates output files */
void printAnswer(int total);

#endif