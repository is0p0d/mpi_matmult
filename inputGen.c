#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
//#include "mpi.h"


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf ("ERROR: Incorrect number of arguments\n");
        printf ("USAGE: ./inputGen n y \n Where n is the size of square array you wish to create, and y is the file you wish to store it in.\n");
        return 1;
    }

    uint64_t numGen = atoi(argv[1]);
    char *outputFileString = argv[2];
    FILE *outputCSV = NULL;

    printf ("WARNING: if \"%s\" exists, it will be overwritten.\n", outputFileString);

    if ((outputCSV = fopen(outputFileString,"w")) == NULL)
    {
        printf("ERROR: Could not open or create output file.\n");
        return 1;
    }
    fprintf(outputCSV, "%llu", numGen);

    for (uint64_t i = 0; i < numGen; i++)
    {
        fprintf(outputCSV,"\n");
        for (uint64_t j = 0; j < numGen; j++)
        {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            srand((time_t)ts.tv_nsec);
            float temp = (float)(rand())/(float)(rand());
            fprintf(outputCSV, "%2.2f,", temp);
        }
    } 
    fclose(outputCSV);
    return 0;
}