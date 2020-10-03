#include "libProcess.h"

#define chunk 1024

void split(char *logfile, int lines, int nmappers)
{

    FILE *file = fopen(logfile, "r");
    int sub_lines = lines / nmappers;

    if (file != NULL && sub_lines > 0)
    {

        char *str = (char *)malloc(chunk);
        int cont_lines = 1;
        int cont_splitFer = 0;
        char *aux = (char *)malloc(10);
        strcpy(aux,"split0.txt");
        FILE *writer;
        int flag = 1;

        while (fgets(str, chunk, file))
        {
            if (flag == 1)
            {
                writer = fopen(aux, "w");
                flag = 0;
            }
            if (cont_lines == sub_lines && cont_splitFer != nmappers - 1)
            {
                cont_lines = 0;
                cont_splitFer++;
                sprintf(aux, "split%d.txt", cont_splitFer);
                flag = 1;
                fprintf(writer, "%s", str);
                fclose(writer);
            }
            else
            {
                fprintf(writer, "%s", str);
            }

            cont_lines++;
        }
        fclose(file);
        fclose(writer);

        free(str);
        free(aux);
    }
    else
    {
        printf("Error\n");
    }
}

void createMappers(int nmappers, char *command, int intermediate){
    int status, i;
    pid_t childpID;
   
    char *splitF = (char *)malloc(5);
    char *extension = (char *)malloc(4);
    char *aux = (char *)malloc(10);
    char *index = (char *)malloc(6);
    strcpy(splitF, "split");
    strcpy(extension, ".txt");
    strcpy(aux, "split0.txt");


    for(i = 0; i < nmappers; i++){

        if((childpID = fork()) < 0){
            perror("Fork: ");
            exit(1);
        }

        mapper(aux, command, childpID, i, intermediate);

        sprintf(index, "%d", i);
        strcpy(aux, splitF);
        strcat(aux, index);
        strcat(aux, extension);
    }

    for(i = 0; i < nmappers ; i++){
        wait(&status);
    }
}

void mapper(char *split, char *command, pid_t childpID, int iter, int intermediate){

    int i, count;
    iter = 0;
    char *pt;
    int col = strtol(command, &pt, 10);
    command = pt + strspn(pt, ",");
    char prueba;

    if(command[0] == '<' && command[1] == '='){
        prueba = '?';
        command = command + strspn(pt, ",");
    } else if (command[0] == '>' && command[1] == '='){
        prueba = '+';
        command = command + strspn(pt, ",");
    } else {
        prueba = command[0];
    }

    char * sign = &prueba;
    command = command + strspn(pt, ",");
    command = command + strspn(pt, ",");
    int value = strtol(command, &pt, 10);
    

    FILE *file = fopen(split, "r");
    char *str = (char *)malloc(chunk);

    char *splitF = (char *)malloc(5);
    char *extension = (char *)malloc(5);
    char *aux = (char *)malloc(10);
    char *index = (char *)malloc(6);
    strcpy(splitF, "buf");
    strcpy(extension, ".txt");
    sprintf(index, "%d", iter);
    strcpy(aux, splitF);
    strcat(aux, index);
    strcat(aux, extension);


    if(file != NULL){
        char *str = (char *)malloc(chunk);

        FILE *writer = fopen(aux, "w");

        while (fgets(str, chunk, file)){        
            count = 0;
            int clave = 0, flag = 0;
            for(;;){
                count ++;
                if(strlen(str)==1)
                    break;
                int cl1 = strtol(str, &pt, 10);
                str = pt + strspn(pt, " ");
                if(flag == 0){
                    clave = cl1;
                    flag = 1;
                }
                if(count == col){
                    if(sign[0] == '>' && cl1 > value){
                        fprintf(writer, "%i %s %i", clave, "\n", cl1);
                    }
                    if(sign[0] == '<' && cl1 < value){
                        fprintf(writer, "%i %s %i", clave, "\n", cl1);
                    }
                    if(sign[0] == '+' && cl1 >= value){
                        fprintf(writer, "%i %s %i", clave, "\n", cl1);
                    }
                    if(sign[0] == '?' && cl1 <= value){
                        fprintf(writer, "%i %s %i", clave, "\n", cl1);
                    }
                    if(sign[0] == '=' && cl1 == value){
                        fprintf(writer, "%i %s %i", clave, "\n", cl1);
                    }
                }

                /*printf("%i %s",cl1, "\n");*/
            }
            /*break;*/
        }
        fclose(writer);
        fclose(file);

        if(intermediate == 0){
            remove(split);
            printf("%s %s", "Se ha borrado ",split);
        }

    } else {
        perror("Error mapper: ");
    }

}

void createReducers(int nreducers, int nmappers){
    int size = nmappers/nreducers;

    int status, i;
    pid_t childpID;
   
    char *splitF = (char *)malloc(5);
    char *extension = (char *)malloc(4);
    char *aux = (char *)malloc(10);
    char *index = (char *)malloc(6);
    strcpy(splitF, "buf");
    strcpy(extension, ".txt");
    strcpy(aux, "buf0.txt");


    for(i = 0; i < nreducers; i++){
/*
        if((childpID = fork()) < 0){
            perror("Fork: ");
            exit(1);
        }

        mapper(aux, command, childpID, i, intermediate);

        sprintf(index, "%d", i);
        strcpy(aux, splitF);
        strcat(aux, index);
        strcat(aux, extension);
*/
    }

    for(i = 0; i < nmappers ; i++){
        wait(&status);
    }

}
