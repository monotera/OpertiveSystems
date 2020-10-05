#===========================Makefile================================#

CC=gcc 
STD= -std=c89
IDIR = common/inc
CFLAGS=-Wall -I$(IDIR)
OBJS = analogp.o analogh.o

all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) $^ -o (EXEC)

analogp.o: analogp.c libs/libProcess.c
    $(CC) (STD) -c $(CFLAGS) analogp.c

analogh.o: analogh.c libs/libThreads.c
    $(CC) (STD) -c $(CFLAGS) analogh.c

clean:
    rm -rf *.o (EXEC) 
