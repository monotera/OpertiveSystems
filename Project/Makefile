all: ProgProcesos ProgHilos

ProgProcesos: analogp.c
	gcc -std=c89 -o ProgProcesos analogp.c libs/libProcess.c

ProgHilos: analogh.c
	gcc -pthread -std=c89  -o  ProgHilos analogh.c libs/libThreads.c
	