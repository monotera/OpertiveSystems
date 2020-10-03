#include "libs/libThreads.h"

/*
    compile file = gcc -pthread -std=c89  -o  analogh analogh.c libs/libThreads.c
*/
int main(int argc, char *argv[])
{
    if (argc != 5)
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
            /*char * command;
            printf("$ ");
            scanf("%s",command);
            /*validate command
            printf("%s %s",command,"\n");*/
            int lines = atoi(argv[2]);
            printf("%d %s",lines,"\n");
            int mappers = atoi(argv[3]);
            split(argv[1],lines,mappers);
            printf("\n");
            break;
        case 2:
            printf("Good bye world\n");
            exit(0);
            break;
        default:
            printf("Error: Invalid selection\n");
            break;
        }
    }
}