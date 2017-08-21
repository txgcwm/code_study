#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define FIFO_NAME       "/tmp/my_fifo"
#define BUFFER_SIZE     PIPE_BUF



int main(int argc, char** argv)
{
    int pipe_fd;
    int res;
    char buffer[BUFFER_SIZE + 1];
    int bytes = 0;

    memset(buffer, '\0', sizeof(buffer));

    printf("Process %d opeining FIFO O_RDONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd != -1) {
        do {
            res = read(pipe_fd, buffer, BUFFER_SIZE);
            bytes += res;
            printf("%d\n", bytes);
        } while(res > 0);

        close(pipe_fd);
    } else {
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished, %d bytes read\n", getpid(), bytes);

    exit(EXIT_SUCCESS);
}

