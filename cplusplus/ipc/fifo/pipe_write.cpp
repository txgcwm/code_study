#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define FIFO_NAME       "/tmp/my_fifo"
#define BUFFER_SIZE     PIPE_BUF
#define TEN_MEG         (1024 * 100)



int main(int argc, char** argv)
{
    int pipe_fd;
    int res;
    int bytes = 0;
    char buffer[BUFFER_SIZE + 1];

    if (access(FIFO_NAME, F_OK) == -1) {
        res = mkfifo(FIFO_NAME, 0777);
        if (res != 0) {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd == -1) {
        exit(EXIT_FAILURE);
    }

    int non_blocking = 1;

    ioctl(pipe_fd, FIONBIO, &non_blocking);

    strcpy(buffer, "test pipe");

    while (bytes < TEN_MEG) {
        res = write(pipe_fd, buffer, BUFFER_SIZE);
        if (res == -1) {
            fprintf(stderr, "Write error on pipe\n");
            exit(EXIT_FAILURE);
        }
        bytes += res;
        printf("%d\n",bytes);
    }

    close(pipe_fd);

    printf("Process %d finish\n", getpid());
    exit(EXIT_SUCCESS);
}

