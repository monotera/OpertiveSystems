#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

/*
     compile file = gcc -std=c89 -o analogh analogh.c
*/

int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        printf("Error : wrong number of parameters\n");
        exit(-1);
    }
    while (1)
    {
        int option;
        printf("1. Make a query\n");
        printf("2. log out\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Hello world\n");
            char * comand;
            printf("$ ");
            scanf("%s",comand);
            printf("%s %s",comand,"\n");
            break;
        case 2:
            printf("Good bye world\n");
            exit(0);
            break;
        default:
            printf("Invalid selection\n");
            break;
        }
    }
}