#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
     int fd;
     char message[100];
     int numchar = 0;
     unlink("aPipe");
     mkfifo("aPipe", 0);
     chmod("aPipe", 460);
     fd = open("aPipe", O_RDONLY);

     while (read(fd, message, 100) > 0)
     {
          printf("%s\n", message);
     }
     close(fd);
}
