all: ProgProcesos ProgHilos

ProgProcesos: analogp.c
	gcc -std=c89 -o analogp analogp.c libs/libProcess.c

ProgHilos: analogh.c
	gcc -pthread -std=c89  -o  analogh analogh.c libs/libThreads.c
	