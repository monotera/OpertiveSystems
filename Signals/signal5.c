#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

int delay;

void childHandler() {

int childpid, status;

childpid= wait(&status);
printf("Child %d terminated within %d seconds", childpid, delay);
exit(0);
}

int main(int argc, char **argv) {
    delay = 2;
    int pid;
    signal(SIGCHLD, childHandler);
    pid = fork();
    if (pid == 0) {
        execvp(argv[2], &argv[2]);
        perror("limit");
   } else {
      sscanf(argv[1], "%d", &delay);
      sleep(delay);
      printf("Child exceeded limit and is being killed\n");
      kill(pid, SIGINT);

  }
}