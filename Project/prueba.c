#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
     compile file = gcc -std=c89 -o analogh analogh.c
*/

int main(int argc, char *argv[])
{

    FILE *fp;
    fp = fopen("test2", "r");
    if (fp == NULL)
    {
        exit - 1;
    }
    int buf[18];
    int i = 0;
    int j = 4;
    while (fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                  &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5],
                  &buf[6], &buf[7], &buf[8], &buf[9], &buf[10], &buf[11],
                  &buf[12], &buf[13], &buf[14], &buf[15], &buf[16], &buf[17]) != EOF)
    {
        printf("%d %d\n", buf[i], buf[j]);
    }
}
