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
            printf("Hello, please insert your command line (column,sign,value)\n$");
            char *command;
            scanf("%s", command);
            int lines = atoi(argv[2]);
            int nmappers = atoi(argv[3]);
            int nreducers = atoi(argv[4]);
            if (nreducers == 0)
            {
                printf("Invalid number of reducers");
            }
            else
            {
                int output;
                output = processControl(argv[1], lines, nmappers, nreducers, command);
                /*output = processControl("test2", 31447, 10, 4, "5,>=,1");*/
                /*output = processControl("test1", 534, 10, 4, "5,>=,1");*/
                if (output >= 0)
                {
                    printf("====> %d\n", output);
                }
            }
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
