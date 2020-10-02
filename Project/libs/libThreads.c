#include "libThreads.h"

#define chunk 1024
map **mappers;
int cont;
int reducer_Answer;

int processControl(char *log, int lines, int nmappers, int nreducers, char *command)
{
    int status = 0;
    intializer(nmappers);
    status = split(log, lines, nmappers);
    if (status)
    {
        return status;
    }
    status = createMappers(nmappers, command);
    if (status)
    {
        return status;
    }
    status = deleteSplit(nmappers);
    if (status)
    {
        return status;
    }
    status = createReducers(nreducers, nmappers);
    if (status)
    {
        return status;
    }
    clear(nmappers);
    return reducer_Answer;
}

void intializer(int nmappers)
{
    mappers = (map **)malloc(nmappers * (sizeof(map *)));
    cont = 0;
    reducer_Answer = 0;
}

int split(char *logfile, int lines, int nmappers)
{

    FILE *file = fopen(logfile, "r");
    int sub_lines = lines / nmappers;

    if (file != NULL && sub_lines > 0)
    {

        char *str = (char *)malloc(chunk);
        int cont_lines = 1;
        int cont_splitFer = 0;
        char *aux = (char *)malloc(10);
        strcpy(aux, "split0.txt");
        FILE *writer;
        int flag = 1;

        while (fgets(str, chunk, file))
        {
            if (flag == 1)
            {
                writer = fopen(aux, "w");
                if (writer == NULL)
                {
                    perror("Error: ");
                    return -1;
                }
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
        perror("Error: ");
        return -1;
    }
    return 0;
}
int createMappers(int nmappers, char *commandM)
{
    int rc;
    pthread_t thread1;
    parameters par;
    int i = 0;
    void * status;
    par.command = (char *)calloc(20, sizeof(command));
    par.split = (char *)calloc(20, sizeof(char) * 20);
    for (i = 0; i < nmappers; i++)
    {
        sprintf(par.split, "split%d.txt", i);
        strcpy(par.command, commandM);
        rc = pthread_create(&thread1, NULL, mapper, (void *)&par);
        if (rc)
        {
            perror("Error: ");
            return -1;
        }
        rc = pthread_join(thread1, NULL);
        if (rc)
        {
            perror("Error: ");
            return -1;
        }
        if(par.status){
            return -1;
        }
    }

    free(par.command);
    free(par.split);
}
void *mapper(void *infor)
{
    struct parameters *info;
    info = (parameters *)infor;
    char *commandM = info->command;
    char *splitFile = info->split;
    info->status = 0;
    
    command com = transform_command(commandM);
    if (com.dif == -1)
    {
        printf("Invalid command!\n");
        info->status = -1;
    }
    FILE *file = fopen(splitFile, "r");
    map x;
    int i = 1;
    int numLines;
    numLines = lineCounter(splitFile);
    if (numLines == -1)
    { 
        info->status = -1;
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
        info->status = -1;
    }

    pthread_exit(NULL);
}
int createReducers(int nreducers, int nmappers)
{
    pthread_t thread1[nreducers];
    int *assignments[nreducers];
    int i = 0, k = 0, j = 0;
    int rc, rj;
    for (i = 0; i < nreducers; i++)
        assignments[i] = (int *)calloc(20, sizeof(int) * 20);
    i = 0;
    while (i < nmappers)
    {
        assignments[j][k] = i;
        assignments[j][k + 1] = -1;
        i++;
        j++;
        if (j == nreducers)
        {
            j = 0;
            k++;
        }
    }
    for (i = 0; i < nreducers; i++)
    {
        rc = pthread_create(&thread1[i], NULL, reducer, (void *)assignments[i]);
         if (rc)
        {
            perror("Error: ");
            return -1;
        }
    }
    for (i = 0; i < nreducers; i++)
    {
        rj = pthread_join(thread1[i], NULL);
        if (rj)
        {
            perror("Error: ");
            return -1;
        }
    }
    return 0;
}

void *reducer(void *assig)
{
    int *assignments = assig;
    int i = 0;
    int index;
    while (assignments[i] != -1)
    {
        index = assignments[i];
        reducer_Answer += mappers[index][0].key - 1;
        i++;
    }
    
}

int deleteSplit(int nmappers)
{
    int i;
    char *aux = (char *)malloc(10);
    strcpy(aux, "split0.txt");
    for (i = 0; i < nmappers; i++)
    {
        int rc = remove(aux);
        if (rc)
        {
            perror("The file couldnt be delete it");
            free(aux);
            return -1;
        }
        sprintf(aux, "split%d.txt", i + 1);
    }
    free(aux);
    return 0;
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
    if (col <= 0 || col > 18) /*Erro message columns cant be negative an 0 means its a letter*/
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
    free(command);
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