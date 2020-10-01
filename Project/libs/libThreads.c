#include "libThreads.h"

#define chunk 1024
map *mappers[chunk];
int cont = 0;
int reducer_Answer = 0;
int reducer_index = 0;

void split(char *logfile, int lines, int nmappers)
{

    FILE *file = fopen(logfile, "r");
    int sub_lines = lines / nmappers;

    if (file != NULL && sub_lines > 0)
    {

        char *str = (char *)malloc(chunk);
        int cont_lines = 1, cont_splitFer = 0;
        char *splitF = (char *)malloc(5);
        char *extension = (char *)malloc(4);
        char *aux = (char *)malloc(10);
        char *index = (char *)malloc(6);
        strcpy(splitF, "split");
        strcpy(extension, ".txt");
        strcpy(aux, "split0.txt");
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
                sprintf(index, "%d", cont_splitFer);
                strcpy(aux, splitF);
                strcat(aux, index);
                strcat(aux, extension);
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
    }
    else
    {
        printf("Error\n");
    }
}
void pruebaImprimir()
{
    int i = 0;
    for (i = 0; i < cont; i++)
    {
        int j = 0;
        printf("-----------------------------------------------\n");
        for (j = 1; j < mappers[i][0].key; j++)
        {
            printf("%d %d \n", mappers[i][j].key, mappers[i][j].value);
        }
    }
}
prueba(char *str, char *str2, char *str3)
{
    createMappers(12, str);
    createMappers(12, str2);
    createMappers(12, str3);
    pruebaImprimir();
    int i;
    for (i = 0; i < cont; i++)
    {
        reducer();
    }
    printf("\n %d", reducer_Answer);
}
void createMappers(int nmappers, char *commandM)
{
    command com = transform_command(commandM);
    if (com.dif == -1)
    {
        printf("-----------------------Error---------------------\n");
    }
    else
    {
        mapper("split0.txt", com.col, com.dif, com.eq);
    }
}
struct command transform_command(char *command)
{
    struct command com;
    char *col = (char *)malloc(strlen(command));
    strcpy(col, command);
    char *token;
    token = strtok(col, ",");
    int place = 0;
    int colum;
    char *dif = (char *)malloc(3);
    int eq;
    while (token != NULL)
    {
        switch (place)
        {
        case 0:
            colum = atoi(token);
            break;
        case 1:
            strcpy(dif, token);
            break;
        case 2:
            eq = atoi(token);
            break;

        default:
            break;
        }
        token = strtok(NULL, ",");
        place++;
    }

    int res = validate_command(colum, dif, eq);

    if (res > 0)
    {
        com.col = colum;
        com.dif = res;
        com.eq = eq;
    }
    else
    {
        com.dif = -1;
    }
    return com;
}
int validate_command(int col, char *dif, int eq)
{
    int validation = -1;
    if (col == 0)
        return validation;
    if (eq == 0) /*revisar*/
        return validation;
    if (strcmp(dif, "<") == 0)
    {
        validation = 1;
    }
    else if (strcmp(dif, ">") == 0)
    {
        validation = 2;
    }
    else if (strcmp(dif, "=") == 0)
    {
        validation = 3;
    }
    else if (strcmp(dif, ">=") == 0)
    {
        validation = 4;
    }
    else if (strcmp(dif, "<=") == 0)
    {
        validation = 5;
    }
    return validation;
}
void mapper(char *split, int col, int dif, int eq)
{
    FILE *file = fopen(split, "r");
    map x;
    int i = 1;
    map *aux_mapper = (map *)calloc(20, sizeof(map) * 20); /*revisa*/
    if (file != NULL)
    {
        char *str = (char *)malloc(chunk);

        while (fgets(str, chunk, file))
        {
            x = line_checker(str, col, dif, eq);
            if (x.value != -163)
            {
                aux_mapper[i] = x;
                i++;
            }
        }

        aux_mapper[0].key = i;
        mappers[cont] = aux_mapper;
        cont++;
    }
    else
    {
        printf("Error\n");
    }
}
struct map line_checker(char *str, int col, int dif, int eq)
{
    map x;
    char *key = (char *)malloc(strlen(str));
    char *value = (char *)malloc(strlen(str));
    strcpy(key, str);
    strcpy(value, str);
    const char s[2] = " ";
    strtok(key, s);
    x.key = atoi(key);

    char *token;
    token = strtok(value, s);
    int i;
    for (i = 0; i < col - 1 && token != NULL; i++)
    {
        token = strtok(NULL, s);
    }
    int val = atoi(token);

    switch (dif)
    {
    case 1:
        if (val < eq)
        {
            x.value = val;
        }
        else
        {
            x.value = -163;
        }
        break;
    case 2:
        if (val > eq)
        {
            x.value = val;
        }
        else
        {
            x.value = -163;
        }
        break;
    case 3:
        if (val == eq)
        {
            x.value = val;
        }
        else
        {
            x.value = -163;
        }
        break;
    case 4:
        if (val >= eq)
        {
            x.value = val;
        }
        else
        {
            x.value = -163;
        }
        break;
    case 5:
        if (val <= eq)
        {
            x.value = val;
        }
        else
        {
            x.value = -163;
        }
        break;

    default:
        break;
    }

    return x;
}

int reducer()
{
    reducer_Answer += mappers[reducer_index][0].key - 1;
    reducer_index++;
}
