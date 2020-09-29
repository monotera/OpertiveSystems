#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/time.h>
#include <unistd.h> 
#include <wait.h>

int  main() {
	struct  timeval  t0 , t1;
	double  media = 0.0;
	int status;
	int i = 0;
	int id =  -1;
	gettimeofday(&t0, NULL);
	for ( i = 0 ; i < 10000 ; i++ ) {
		id = fork ();
		if (id == 0) {
			gettimeofday(&t1, NULL);
			unsigned  int  ut1 = t1.tv_sec *1000000+ t1.tv_usec ;
			unsigned  int  ut0 = t0.tv_sec *1000000+ t0.tv_usec ;
			media  += (ut1 -ut0 );
			exit(0);
		}
		if (id != 0) {
			wait(&status);
		}
	}
printf (" fin del programa\n") ;
}