#include <stdlib.h>
#include <stdio.h>

#include "parser.h"



void usage(char *program_name)
{
    printf("Usage: %s [input.flv]\n", program_name);
    
    return;
}

int main(int argc, char **argv) 
{

    FILE *infile = NULL;

    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    infile = fopen(argv[1], "r");
    if (infile == NULL) {
        usage(argv[0]);
        return -1;
    }

    flv_parser_run(infile);

    printf("\nFinished analyzing\n");

    return 0;
}


