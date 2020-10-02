#include "libThreads.h"

#define chunk 1024
map **mappers;
int cont;
int reducer_Answer;
int reducer_index;

void intializer(int nmappers)
{
    mappers = (map **)malloc(nmappers * (sizeof(map *)));
    cont = 0;
    reducer_Answer = 0;
    reducer_index = 0;
}

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
                /*sprintf(index, "%d", cont_splitFer);
                strcpy(aux, splitF);
                strcat(aux, index);
                strcat(aux, extension);*/
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

int deleteSplit(int nmappers)
{
    int i;
    char *splitF = (char *)malloc(5);
    char *extension = (char *)malloc(4);
    char *aux = (char *)malloc(10);
    char *index = (char *)malloc(6);
    strcpy(splitF, "split");
    strcpy(extension, ".txt");
    strcpy(aux, "split0.txt");
    for (i = 0; i < nmappers; i++)
    {
        int rc = remove(aux);
        if (rc)
        {
            perror("The file couldnt be delete it");
            free(splitF);
            free(extension);
            free(aux);
            free(index);
            return -1;
        }
        sprintf(index, "%d", i + 1);
        strcpy(aux, splitF);
        strcat(aux, index);
        strcat(aux, extension);
    }
    free(splitF);
    free(extension);
    free(aux);
    free(index);
    return 0;
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

void prueba(char *command, char *log, int lines, int nmappers)
{
    intializer(nmappers);
    split(log, lines, nmappers);

    createMappers(nmappers, command);
    deleteSplit(nmappers);
    pruebaImprimir();
    int i;
    for (i = 0; i < cont; i++)
    {
        reducer();
    }
    printf("\n %d", reducer_Answer);
    clear(nmappers);
}
void createMappers(int nmappers, char *commandM)
{
    mapper("test1", commandM);
}
void mapper(char *split, char *commandM)
{
    command com = transform_command(commandM);
    if (com.dif == -1)
    {
        printf("-----------------------Error---------------------\n");
        return;
    }
    FILE *file = fopen(split, "r");
    map x;
    int i = 1;
    int numLines;
    numLines = lineCounter(split);
    if (numLines == -1)
    { /*Error*/
        return;
    }
    map *aux_mapper = (map *)calloc(numLines, sizeof(map) * 20);
    if (file != NULL)
    {
        char *str = (char *)malloc(chunk);

        while (fgets(str, chunk, file))
        {
            x = line_checker(str, com.col, com.dif, com.eq);
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
    int eq, flag = 0;
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
            if (strcmp(token, "0") == 0) /*Its 0 so the validation is already done*/
                flag = 1;
            eq = atoi(token);
            break;

        default:
            break;
        }
        token = strtok(NULL, ",");
        place++;
    }

    int res = validate_command(colum, dif, eq, flag);

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
int validate_command(int col, char *dif, int eq, int flag)
{
    int validation = -1;
    if (col < 0) /*Erro message columns cant be negative an 0 means its a letter*/
        return validation;
    if (eq == 0 && flag == 0)
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

int lineCounter(char *log)
{
    char *command = (char *)calloc(20, sizeof(log) * 20);
    FILE *fp1;
    int lines;
    sprintf(command, "cat %s | wc -l > lineCounterAux.txt", log);
    system(command);
    fp1 = fopen("lineCounterAux.txt", "r");
    if (fp1 == NULL)
    {
        perror("Error: ");
        return (-1);
    }
    fscanf(fp1, "%d", &lines);
    fclose(fp1);
    remove("lineCounterAux.txt");
    return lines;
}

void clear(int nmappers)
{
    int i;
    for (i = 0; i < nmappers; i++)
    {
        free(mappers[i]);
    }
    free(mappers);
}