/**************************************
 * Jim M                         2024 *
 * CSC6780 - Distributed Computing    *
 * Due: 02.24.24                      *
 * prob5.c                            *
 * A basic mpi matrix multiply        *
 * program                            *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include "mpi.h"

#define BUFFERSIZE 32000

void arrPrint(uint64_t size, float array[][size]);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf ("ERROR: Incorrect number of arguments\n");
        printf ("USAGE: problem5 n input.dat output.dat \n Where n is the number of processors you wish to use.\n");
        return 1;
    }
    
    uint64_t numProcs = atoi(argv[1]);
    char *inputFileString = argv[2];
    char *outputFileString = argv[3];
    FILE *inputCSV = NULL;
    FILE *outputCSV = NULL;

    if ((inputCSV = fopen(inputFileString, "r")) == NULL)
    {
        printf("ERROR: Could not open input file.\n");
        return 1;
    }

    printf ("WARNING: if \"%s\" exists, it will be overwritten.\n", outputFileString);

    if ((outputCSV = fopen(outputFileString,"w")) == NULL)
    {
        printf("ERROR: Could not open or create output file.\n");
        return 1;
    }

    /* Populate array from input file */
    /* First line should be array size */
    char tempRead[BUFFERSIZE] = {'\0'};
    char *valToken = NULL;
    fgets(tempRead, BUFFERSIZE, inputCSV);

    int arrSize = atoi(tempRead);
    printf("Array size of %d determined, creating and reading arrays...\n", arrSize);
    float arrA[arrSize][arrSize],
          arrB[arrSize][arrSize],
          arrC[arrSize][arrSize];
    


    // after original fgets the file pointer should now be positioned at the start of the array data
    int rowCount = 0,
        colCount = 0;
    while ((fgets(tempRead, BUFFERSIZE, inputCSV)) != NULL)
    {
        valToken = strtok(tempRead, ",");
        colCount = 0;
        while (valToken != NULL)
        {
            float tempFloat = atof(valToken);
            printf("%d,%d: %f\n", rowCount, colCount, tempFloat);
            
            arrA[rowCount][colCount] = tempFloat;
            arrB[rowCount][colCount] = tempFloat;

            valToken = strtok(NULL, ",");
            colCount++;
        }
        rowCount++;
    }
    printf("arrA:\n");
    arrPrint(arrSize, arrA);

    printf("arrB :\n");
    arrPrint(arrSize, arrB);

    return 0;
}

void arrPrint(uint64_t size, float array[][size])
{
    for (int i = 0; i < size; i++)
    {
        printf("[");
        for (int j = 0; j < size; j++)
        {
            printf("%f\t,", array[i][j]);
        }
        printf("]\n");
    }
}