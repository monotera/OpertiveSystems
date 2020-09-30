#include "libProcess.h"

void split(char *logfile, int lines, int nmappers){

    FILE * file = fopen(logfile,"r");
    if(file != NULL){
        char * str;
        while(fgets(str,lines,file)){
            printf(str);
        }
    }
    else{
        printf("Error\n");
    }
} 