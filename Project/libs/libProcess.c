/*issue : cuanto la division da decimal, siempre habran mas lineas que nmpares?*/
#include "libProcess.h"

#define chunk 1024

void split(char *logfile, int lines, int nmappers)
{

    FILE *file = fopen(logfile, "r");
    if (file != NULL)
    {
        char *str = (char *)malloc(chunk);
        int sub_lines = lines / nmappers;
        int close_flag = lines % nmappers;
        printf("%d %s", sub_lines, "\n");
        int cont_lines = 1, cont_buffer = 0;
        char *buff = (char *)malloc(4);
        char *extension = (char *)malloc(4);
        char *buffname = (char *)malloc(9);
        char *aux = (char *)malloc(9);
        char *index = (char *)malloc(6);
        strcpy(buff, "buff");
        strcpy(extension, ".txt");
        strcpy(buffname, "buff0.txt");
        FILE *writer;
        int flag = 1;

        while (fgets(str, chunk, file))
        {
           /* printf("%d %s %d %s", cont_lines, " == ", sub_lines, "\n");*/
            if (cont_lines == sub_lines)
            {
                cont_lines = 0;
                cont_buffer++;
                sprintf(index, "%d", cont_buffer);
                strcpy(aux, buff);
                strcat(aux, index);
                strcat(aux, extension);
                strcpy(buffname, aux);
                flag = 1;
                fprintf(writer, "%s", str);
                printf("%s %s %s", "linea ", str, "\n");
                fclose(writer);
                printf("-----------------cerrar----------------\n");
            }
            else
            {
                if (flag == 1)
                {
                    printf("-----------------abrir----------------\n");
                    writer = fopen(buffname, "w");
                    flag = 0;
                }
                fprintf(writer, "%s", str);
                printf("%s %s %s", "linea ", str, "\n");
            }
            cont_lines++;
        }
        fclose(file);
        if(close_flag != 0){
            fclose(writer);
        }
        
    }
    else
    {
        printf("Error\n");
    }
}
