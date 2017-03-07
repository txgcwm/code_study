#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static void generateRandom(char *buf, int len)
{
	bzero(buf, len);

	srand((int)time(NULL));

    for (int i = 0; i < len - 1; i++) {
        buf[i] = 'a' + (0 + (int)(26.0 * rand()/(RAND_MAX + 1.0)));
    }

    buf[len - 1] = '\0';

	return;
}

int main(int argc, char *argv[])
{
    int len = 20;
    char buf[20];

    generateRandom(buf, len);
    
    printf("The randm String is [ %s ]\n", buf);

    return 0;
}
