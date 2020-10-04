#include "libProcess.h"

#define chunk 1024

int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int status = 0;
    status = split(log, lines, nmappers);
    if (status)
    {
        return status;
    }
    struct command com;
    com = transform_command(command);
    if (com.dif == -1)
    {
        printf("Error: invalid command\n");
        deleteFiles(nmappers, "split");
        return -1;
    }
    status = createMappers(nmappers, com, inter);
    if (inter == 0)
    {
        status = deleteFiles(nmappers, "split");
        if (status)
        {
            return status;
        }
    }
    if (status)
    {
        deleteFiles(nmappers, "split");
        return status;
    }
    status = createReducers(nreducers, nmappers);
    if (inter == 0)
    {
        status = deleteFiles(nmappers, "buff");
        if (status)
        {
            return status;
        }
    }
    if (status)
    {
        return status;
    }
    if (status)
    {
        deleteFiles(nmappers, "split");
        return status;
    }
    status = printAnswer(nreducers);
    if (inter == 0)
    {
        deleteFiles(nreducers, "output");
    }
    if (status == -1)
    {
        return status;
    }
    gettimeofday(&end, NULL);
    printf("Time of execution: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
    return status;
}
int split(char *logfile, int lines, int nmappers)
{

    FILE *file;
    int sub_lines, cont_lines, cont_splitFer, flag;
    char *str, *aux;
    file = fopen(logfile, "r");
    sub_lines = lines / nmappers;

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
                    printf("Error: Writer couldn't be open\n");
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
        printf("Error: File couldn't be open\n");
        return -1;
    }
    return 0;
}
int createMappers(int nmappers, command command, int intermediate)
{
    int status, i, stat = 0;
    pid_t pidC;

    char *aux = (char *)malloc(10);
    strcpy(aux, "split0.txt");

    for (i = 0; i < nmappers; i++)
    {
        pidC = fork();
        if (pidC > 0)
        {
            continue;
        }
        else if (pidC == 0)
        {
            sprintf(aux, "split%d.txt", i);
            stat = mapper(aux, command, i, intermediate);
            if (stat)
            {
                return -1;
            }
            exit(0);
        }
        else
        {
            perror("Fork: ");
            exit(-1);
        }
    }

    for (i = 0; i < nmappers; i++)
    {
        pidC = wait(&status);
        /*printf("P! padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidC);*/
        if (pidC == -1)
            stat = -1;
    }
    return stat;
}
int mapper(char *split, command com, int iter, int intermediate)
{
    map x;

    char *buff = (char *)calloc(20, sizeof(char) * 20);
    sprintf(buff, "buff%d.txt", iter);
    FILE *writer = fopen(buff, "w");
    FILE *file = fopen(split, "r");
    if (file != NULL && writer != NULL)
    {
        int h = com.col;
        int buf[19];
        while (fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                      &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6],
                      &buf[7], &buf[8], &buf[9], &buf[10], &buf[11], &buf[12],
                      &buf[13], &buf[14], &buf[15], &buf[16], &buf[17], &buf[18]) != EOF)
        {
            x.value = -163;
            x.key = buf[1];
            switch (com.dif)
            {
            case 1:
                if (buf[h] < com.eq)
                {
                    x.value = buf[h];
                }
                break;
            case 2:
                if (buf[h] > com.eq)
                {
                    x.value = buf[h];
                }
                break;
            case 3:
                if (buf[h] == com.eq)
                {
                    x.value = buf[h];
                }
                break;
            case 4:
                if (buf[h] >= com.eq)
                {
                    x.value = buf[h];
                }
                break;
            case 5:
                if (buf[h] <= com.eq)
                {
                    x.value = buf[h];
                }
                break;
            default:
                break;
            }
            if (x.value != -163)
            {
                fprintf(writer, "%d %d \n", x.key, x.value);
            }
        }
    }
    else
    {
        printf("Error\n");
        return -1;
    }
    fclose(writer);
    fclose(file);
    return 0;
}
int deleteFiles(int canti, char *type)
{
    int i;
    char *aux = (char *)malloc(10);
    sprintf(aux, "%s%d.txt", type, 0);
    for (i = 0; i < canti; i++)
    {
        int rc = remove(aux);
        if (rc)
        {
            perror("The file couldnt be delete it");
            free(aux);
            return -1;
        }
        sprintf(aux, "%s%d.txt", type, i + 1);
    }
    free(aux);
    return 0;
}

int clear(int nmappers, int nreducers)
{
    int status;
    status = deleteFiles(nmappers, "split");
    if (status)
    {
        return -1;
    }
    status = deleteFiles(nmappers, "buff");
    if (status)
    {
        return -1;
    }
    status = deleteFiles(nreducers, "output");
    if (status)
    {
        return -1;
    }
    return 0;
}
int printAnswer(int nreducers)
{
    int i = 0;
    char *out = (char *)calloc(20, sizeof(char) * 20);
    int sum = 0;
    int aux;
    FILE *fp;
    for (i = 0; i < nreducers; i++)
    {
        sprintf(out, "output%d.txt", i);
        fp = fopen(out, "r");
        if (fp != NULL)
        {
            fscanf(fp, "%d", &aux);
            sum += aux;
        }
        else
        {
            perror("Error:");
            return -1;
        }
        fclose(fp);
    }
    return sum;
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

int createReducers(int nreducers, int nmappers)
{
    int *assignments[nreducers];
    int i = 0, k = 0, j = 0, z, y;
    int stat = 0, status;
    pid_t pidE;

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

    for (z = 0; z < nreducers; z++)
    {
        pidE = fork();
        if (pidE > 0)
        {
            continue;
        }
        if (pidE == 0)
        {
            stat = reducer(1, assignments[z], z);
            if (stat)
            {
                return -1;
            }
            exit(0);
        }
        else
        {
            perror("Fork: ");
            exit(-1);
        }
    }

    for (y = 0; y < nreducers; y++)
    {
        pidE = wait(&status);
        if (status)
            stat = -1;

        /*printf("P2 padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidE);*/
    }
    return stat;
}

int reducer(int intermediate, int *assignments, int index)
{
    int i = 0, suma = 0;
    char *buff = (char *)calloc(20, sizeof(char));
    char *out = (char *)calloc(20, sizeof(char));
    sprintf(out, "output%d.txt", index);
    FILE *writer;
    FILE *fp;

    while (assignments[i] != -1)
    {
        char aux;

        sprintf(buff, "buff%d.txt", assignments[i]);
        fp = fopen(buff, "r");
        if (fp != NULL)
        {
            aux = getc(fp);
            while (aux != EOF)
            {
                if (aux == '\n')
                {
                    suma++;
                }
                aux = getc(fp);
            }
        }
        else
        {
            perror("Error: ");
        }

        i++;
        fclose(fp);
    }
    writer = fopen(out, "w");
    if (writer == NULL)
    {
        perror("Error: ");
        return -1;
    }
    fprintf(writer, "%d", suma);
    fclose(writer);
    return 0;
}