#include "libs/libProcess.h"

/*
     compile file = gcc -std=c89 -o analogp analogp.c libs/libProcess.c
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

            printf("Hello!, Checking for files deletions!\n");
            int lines = atoi(argv[2]);
            int nmappers = atoi(argv[3]);
            int nreducers = atoi(argv[4]);
            int inter = atoi(argv[5]);
            if (inter == 1)
            {
                clear(nmappers, nreducers);
            }
            printf("Done!, please insert your command line (column,sign,value)\n$");
            char *command;
            scanf("%s", command);

            if (nreducers == 0)
            {
                printf("Invalid number of reducers");
            }
            if (inter > 1 || inter < 0)
            {
                printf("Error: inter must be 0 or 1");
                exit - 1;
            }
            else
            {

                int output;
                output = processControl(argv[1], lines, nmappers, nreducers, command, inter);
                /*output = processControl("test1.txt", 534, 10, 3, "5,>=,1", 0);*/
                if (output >= 0)
                {
                    printf("there are %d records that meet the condition\n", output);
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
