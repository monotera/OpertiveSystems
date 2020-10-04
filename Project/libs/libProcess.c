#include "libProcess.h"

#define chunk 1024

void split(char *logfile, int lines, int nmappers)
{

    FILE *file, *writer;
    int sub_lines, cont_lines, cont_splitFer, flag;
    char *str, *aux;
    file = fopen(logfile, "r");
    int sub_lines = lines / nmappers;

    if (file != NULL && sub_lines > 0)
    {

        str = (char *)malloc(chunk);
        cont_lines = 1;
        cont_splitFer = 0;
        aux = (char *)malloc(10);
        strcpy(aux, "split0.txt");
        //FILE *writer;
        flag = 1;

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
}
int deleteFiles(int canti, char *type)
{
    int i, rc;
    char *aux = (char *)malloc(10);

    sprintf(aux, "%s%d.txt", type, 0);
    for (i = 0; i < canti; i++)
    {
        rc = remove(aux);
        if (rc)
        {
            perror("The file couldn't be deleted");
            free(aux);
            return -1;
        }
        sprintf(aux, "%s%d.txt", type, i + 1);
    }
    free(aux);
    return 0;
}
int processControl(char *log, int lines, int nmappers, int nreducers, char *command, int inter)
{
    int status = 0;

    split(log, lines, nmappers);
    createMappers(nmappers, command, inter);
    if (inter == 0)
    {
        deleteFiles(nmappers, "split");
    }
    createReducers(nreducers, nmappers);
    if (inter == 0)
        deleteFiles(nmappers, "buff");
    status = printAnswer(nreducers);
    if (inter == 0)
        deleteFiles(nreducers, "output");
    return status;
}
void clear(int nmappers, int nreducers)
{
    deleteFiles(nmappers, "split");
    deleteFiles(nmappers, "buff");
    deleteFiles(nreducers, "output");
}
int printAnswer(int nreducers)
{
    int i, sum = 0;
    int aux;
    char *out = (char *)calloc(20, sizeof(char) * 20);
    FILE *fp;


    for (i ; i < nreducers; i++)
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

void createMappers(int nmappers, char *command, int intermediate)
{
    int status, i;
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
            mapper(aux, command, i, intermediate);
            exit(1);
        }
        else
        {
            perror("Fork: ");
            return -1;
        }
    }

    for (i = 0; i < nmappers; i++)
    {
        pidC = wait(&status);
        /*printf("P! padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidC);*/
    }
}
struct command transform_command(char *command)
{
    struct command com;
    char *col = (char *)malloc(strlen(command));
    strcpy(col, command);
    char *token;
    token = strtok(col, ",");
    int place, eq, flag = 0;
    int colum, res;
    char *dif = (char *)malloc(3);

   
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

    res = validate_command(colum, dif, eq, flag);

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
void mapper(char *split, char *command, int iter, int intermediate)
{
    map x;
    struct command com;
    com = transform_command(command);
    int h, buf[19];
    FILE *file, *writer;
    file = fopen(split, "r");
    char *buff = (char *)calloc(20, sizeof(char) * 20);
    sprintf(buff, "buff%d.txt", iter);
    writer = fopen(buff, "w");
    if (file != NULL)
    {
        h = com.col;
        //int buf[19];
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
        perror("Error: ");
        return -1;
    }
    fclose(writer);
    fclose(file);
}

void createReducers(int nreducers, int nmappers)
{
    int *assignments[nreducers];
    int i = 0, k = 0, j = 0, z = 0;
    int status, y;
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

   
    for (z; z < nreducers; z++)
    {
        pidE = fork();
        if (pidE > 0)
        {
            continue;
        }
        if (pidE == 0)
        {
            reducer(1, assignments[z], z);
            exit(0);
        }
        else
        {
            perror("Fork: ");
            return -1;
        }
    }

    for (y; y < nreducers; y++)
    {
        pidE = wait(&status);
        /*printf("P2 padre de PID  = %d, hijo de pid = %d terminado\n", getpid(), pidE);*/
    }
}

int reducer(int intermediate, int *assignments, int index)
{
    int i = 0, suma = 0;
    char aux;
    char *buff = (char *)calloc(20, sizeof(log) * 20);
    char *out = (char *)calloc(20, sizeof(log) * 20);
    sprintf(out, "output%d.txt", index);
    FILE *writer = fopen(out, "w");
    while (assignments[i] != -1)
    {
        

        sprintf(buff, "buff%d.txt", assignments[i]);
        FILE *fp = fopen(buff, "r");
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
            return -1;
        }

        i++;
        fclose(fp);
    }
    fprintf(writer, "%d", suma);
    fclose(writer);
}