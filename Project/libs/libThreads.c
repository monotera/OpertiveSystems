#include "libThreads.h"

#define chunk 1024

int cont;
int reducer_Answer;

int processControl(char *log, int lines, int nmappers, int nreducers, char *command)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int status = 0;
    intializer(nmappers);
    status = split(log, lines, nmappers);
    if (status)
    {
        return status;
    }
    map *p = calloc(nmappers, sizeof(map));
    p = createMappers(nmappers, command);
    int i = 0;
    int j = 0;
    int g = 0;
    if (status)
    {
        return status;
    }
    status = deleteSplit(nmappers);
    if (status)
    {
        return status;
    }
    status = createReducers(nreducers, nmappers, p);
    if (status)
    {
        return status;
    }
    /*clear(nmappers);*/

    /*gettimeofday(&end, NULL);
    printf("Time of execution: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));*/
   /* return reducer_Answer;*/
}

void intializer(int nmappers)
{
    /*mappers = (map **)malloc(nmappers * (sizeof(map *)));*/
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

map *createMappers(int nmappers, char *commandM)
{
    int rc, rj;
    pthread_t thread1[nmappers];
    map m[nmappers];
    int i = 0;
    void *status;
    parameters par[nmappers];
    for (i = 0; i < nmappers; i++)
    {
        par[i].split = (char *)calloc(chunk, sizeof(char) * chunk);
        sprintf(par[i].split, "split%d.txt", i);
        par[i].com = transform_command(commandM);
        if (par[i].com.dif == -1)
        {
            printf("Invalid command!\n");
            deleteSplit(nmappers);
            return -1;
        }
        par[i].numLines = lineCounter(par[i].split);
        par[i].m.key = calloc(par[i].numLines + 1, sizeof(int));
        par[i].m.value = calloc(par[i].numLines + 1, sizeof(int));
        /*par[i].m = m[i];*/
    }

    int j = 0;
    for (j = 0; j < nmappers; j++)
    {
        rc = pthread_create(&thread1[j], NULL, mapper, (void *)&par[j]);
        if (rc)
        {
            perror("Error: ");
            return -1;
        }
        if (par[i].numLines == -1)
        {
            return -1;
        }
    }
    int k = 0;
    for (k = 0; k < nmappers; k++)
    {
        rj = pthread_join(thread1[k], NULL);
        if (rj)
        {
            perror("Error: ");
            return -1;
        }
    }
    map *p = calloc(nmappers, sizeof(map));
    int g = 0;

    for (i = 0; i < nmappers; i++)
    {
        /*parameters p;*/
        p[i] = par[i].m;
        /*int index = p.m.key[0];
        printf("====> %d\n",index);
        for ( j = 1; j < index; j++)
        {
            printf("Key: %d, value: %d\n",p.m.key[j],p.m.value[j]);
            g++;
        }*/
    }
    /*for ( i = 0; i < nmappers; i++)
    {
        int index = p[i].key[0];
        printf("====> %d\n",index);
        for ( j = 1; j < index; j++)
        {
            printf("Key: %d, value: %d\n",p[i].key[j],p[i].value[j]);
            g++;
        }
        
    }*/

    printf("%d ", g);
    return p;
}

void *mapper(void *infor)
{
    struct parameters *info;
    info = (parameters *)infor;
    char *splitFile = info->split;
    info->status = 0;

    FILE *file = fopen(splitFile, "r");

    int i = 1;
    map *aux_mapper = (map *)calloc(info->numLines, sizeof(map) * info->numLines);
    if (file != NULL)
    {
        char *str = (char *)malloc(chunk);
        int h = info->com.col;
        int buf[19];
        int z = 1;
        while (fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                      &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6],
                      &buf[7], &buf[8], &buf[9], &buf[10], &buf[11], &buf[12],
                      &buf[13], &buf[14], &buf[15], &buf[16], &buf[17], &buf[18]) != EOF)
        {

            switch (info->com.dif)
            {
            case 1:
                if (buf[h] < info->com.eq)
                {
                    info->m.key[z] = buf[1];
                    info->m.value[z] = buf[h];
                    z++;
                }
                break;
            case 2:
                if (buf[h] > info->com.eq)
                {
                    info->m.key[z] = buf[1];
                    info->m.value[z] = buf[h];
                    z++;
                }
                break;
            case 3:
                if (buf[h] == info->com.eq)
                {
                    info->m.key[z] = buf[1];
                    info->m.value[z] = buf[h];
                    z++;
                }
                break;
            case 4:
                if (buf[h] >= info->com.eq)
                {
                    info->m.key[z] = buf[1];
                    info->m.value[z] = buf[h];
                    z++;
                }

                break;
            case 5:
                if (buf[h] <= info->com.eq)
                {
                    info->m.key[z] = buf[1];
                    info->m.value[z] = buf[h];
                    z++;
                }
                break;

            default:
                break;
            }
        }
        info->m.key[0] = z;
    }
    else
    {
        printf("Error\n");
        info->status = -1;
    }

    pthread_exit(NULL);
}
int createReducers(int nreducers, int nmappers, map *p)
{

    pr par[nreducers];
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

    int suma = 0;
    for (i = 0; i < nreducers; i++)
    {
        par[i].assigments = assignments[i];
        par[i].m = p;
        par[i].res = 0;
        rc = pthread_create(&thread1[i], NULL, reducer, (void *)&par[i]);
        if (rc)
        {
            perror("Error: ");
            return -1;
        }
        suma += par[i].res;
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
    printf("\n  ===res===> %d", suma);
    return 0;
}

void *reducer(void *assig)
{
    struct pr *info;
    info = (pr *)assig;
    int i = 0;
    map *p = info->m;
    int j = 0;
    while (info->assigments[i] != -1)
    {
        int indej = info->assigments[i];
        info->res += p[indej].key[0] - 1;
        i++;
    }
    printf("\n");
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

/*void clear(int nmappers)
{
    int i;
    for (i = 0; i < nmappers; i++)
    {
        free(mappers[i]);
    }
    free(mappers);
}*/