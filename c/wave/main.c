#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "wave_header.h"
#include "wave_parse.h"

#define BUFFER_SIZE     1024
#define FILENAME_LENGTH 60

void usage(char *app_name)
{
    printf("%s -a action -i input filename -o output filename -t formatTag -s sampleRate -b bitPerSample\n", (char *)basename(app_name));
    printf(
        "action:\n"
        "\t 0: parse wave header\n"
        "\t./wave -a 0 -i in_filename\n\n"
        "\t 1: add wave header\n"
        "\t./wave -a 1 -i in_filename -o out_filename -t 69 -s 8000 -b 4\n\n"
        "\t 2: add amr header\n"
        "\t./wave -a 2 -i in_filename -o out_filename\n\n"
    );
}

void parse_wave_file(char *infile)
{
	wav_t *wav = NULL;

    if(infile == NULL || strlen(infile) <= 0) {
        printf("\terror\n");
        return;
    }

    wav = wav_open(infile);
    if(NULL != wav){
        wav_dump(wav);
        wav_close(&wav);
    }

	return;
}

void copy_file(int from_fd, int to_fd)
{
    int bytes_read, bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr = NULL;

    while ((bytes_read = read(from_fd, buffer, BUFFER_SIZE)) > 0) {
        if ((bytes_read == -1) && (errno != EINTR)) 
            break;
        else if (bytes_read > 0) {
            ptr = buffer;
            while ((bytes_write = write(to_fd, ptr, bytes_read)) > 0) {
               
                if ((bytes_write == -1) && (errno != EINTR))
                    break;
                else if (bytes_write == bytes_read) 
                    break;
                else if (bytes_write > 0) {
                    ptr += bytes_write;
                    bytes_read -= bytes_write;
                }
            }

            if (bytes_write == -1)
                break;
        }
    }

    return;
}

void add_wave_header(char *infile, char *outfile, short formatTag, int sampleRate, short bitPerSample)
{
	struct stat buf;
	int from_fd, to_fd;
	void *header = NULL;

	if(infile == NULL || strlen(infile) <= 0 || outfile == NULL || strlen(outfile) <= 0) {
        printf("error\n");
        return;
    }

    stat(infile, &buf);
    if(buf.st_size <= 0) {
        return;
    }

	printf("infile name: %s, source file length: %ld\n", infile, buf.st_size);
	
    if ((from_fd = open(infile, O_RDONLY)) == -1) {
        fprintf(stderr, "Open Error：%s\n", strerror(errno));
        return;
    }

    if ((to_fd = open(outfile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        fprintf(stderr, "Open Error：%s\n", strerror(errno));
        return;
    }

    header = createWaveHeader(buf.st_size, formatTag, 1, sampleRate, bitPerSample);
    if(header == NULL) {
        printf("can not create wave header!\n");
        return;
    }
	write(to_fd, header, sizeof(struct wave_header));
    free(header);

    copy_file(from_fd, to_fd);

    close(from_fd);
    close(to_fd);

	return;
}

void add_amr_header(char *infile, char *outfile)
{
    struct stat buf;
    int from_fd,to_fd;
    char header[] = {0x23, 0x21, 0x41, 0x4D, 0x52, 0x0A};

    if(infile == NULL || strlen(infile) <= 0 || outfile == NULL || strlen(outfile) <= 0) {
        printf("error\n");
        return;
    }

    stat(infile, &buf);
    if(buf.st_size <= 0)
        return;

    if((from_fd=open(infile, O_RDONLY)) == -1) {
        fprintf(stderr,"Open Error:%s\n", strerror(errno));
        return;
    }

    if((to_fd = open(outfile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        fprintf(stderr,"Open Error:%s\n", strerror(errno));
        return;
    }

    write(to_fd, header, sizeof(header));

    copy_file(from_fd, to_fd);

    close(from_fd);
    close(to_fd);

    return;
}

int main(int argc, char **argv){

	int res = 0;
	int action = 0;
    int sampleRate = 32000;
    short formatTag = 1, bitPerSample = 4;
	char in[FILENAME_LENGTH] = {0};
	char out[FILENAME_LENGTH] = {0};

    if(argc < 5) {
        usage(argv[0]);
        return -1;
    }

    while((res = getopt(argc, argv, "?a:i:o:t:s:b:h")) != -1) {
        switch(res) {
        case 'a':
            action = atoi(optarg);
            break;

        case 'i':
            memcpy(in, optarg, strlen(optarg));
            break;

		case 'o':
            memcpy(out, optarg, strlen(optarg));
            break;

        case 't':
            formatTag = atoi(optarg);
            break;

        case 's':
            sampleRate = atoi(optarg);
            break;

        case 'b':
            bitPerSample = atoi(optarg);
            break;

        case 'h':
        default:
            usage(argv[0]);
            return -1;
        }
    }

	switch(action) {
		case 0:
			parse_wave_file(in);
			break;

		case 1:
			add_wave_header(in, out, formatTag, sampleRate, bitPerSample);
			break;

        case 2:
            add_amr_header(in, out);
            break;
	}
   
    return 0;
}
