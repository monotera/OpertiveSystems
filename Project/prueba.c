#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
     compile file = gcc -std=c89 -o analogh analogh.c
*/

int main(int argc, char *argv[])
{
    char *comando = (char *)calloc(20,sizeof(char) * 20);
    FILE*fp1;
    int lineas;
    sprintf(comando, "cat %s | wc -l > valor.txt", "test1");
    printf("comando = %s\n", comando);
}
