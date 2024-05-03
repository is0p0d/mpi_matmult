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
#include "mpi.h"

#define BUFFERSIZE 32000

void arrPrint(uint64_t size, float (*array));

int main(int argc, char* argv[])
{
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 3)
    {
        printf ("ERROR: Incorrect number of arguments\n");
        printf ("USAGE: problem5 n input.dat output.dat \n Where n is the number of processors you wish to use.\n");
        MPI_Finalize();
        return 1;
    }

    uint64_t numProcs = atoi(argv[1]);
    int arrSize = 0;
    float *arrA = NULL,
          *arrB = NULL,
          *arrC = NULL;
    
    if(rank == 0)
    {
        /***************************************************************/
        /* INPUT SECTION ***********************************************/
        /***************************************************************/

        char *inputFileString = argv[2];
        char *outputFileString = argv[3];
        FILE *inputCSV = NULL;
        FILE *outputCSV = NULL;

        if ((inputCSV = fopen(inputFileString, "r")) == NULL)
        {
            printf("ERROR: Could not open input file.\n");
            MPI_Finalize();
            return 1;
        }

        printf ("WARNING: if \"%s\" exists, it will be overwritten.\n", outputFileString);

        if ((outputCSV = fopen(outputFileString,"w")) == NULL)
        {
            printf("ERROR: Could not open or create output file.\n");
            MPI_Finalize();
            return 1;
        }

        /* Populate array from input file */
        /* First line should be array size */
        char tempRead[BUFFERSIZE] = {'\0'};
        char *valToken = NULL;
        fgets(tempRead, BUFFERSIZE, inputCSV);

        arrSize = atoi(tempRead);
        printf("Array size of %d determined, creating and reading arrays...\n", arrSize);
        float t_arrA[arrSize][arrSize],
              t_arrB[arrSize][arrSize],
              t_arrC[arrSize][arrSize];
        arrA = &((*t_arrA)[0]); // this feels offensive
        arrB = &((*t_arrB)[0]);
        arrC = &((*t_arrC)[0]);

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
                
                (&arrA)[rowCount][colCount] = tempFloat;
                (&arrB)[rowCount][colCount] = tempFloat;

                valToken = strtok(NULL, ",");
                colCount++;
            }
            rowCount++;
        }
    }
    else if(rank != 0)
    {
        printf("Hi from sunny processor number: %d", rank);
    }
    arrPrint(arrSize, arrA);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

void arrPrint(uint64_t size, float (*array))
{
    for (int i = 0; i < size; i++)
    {
        printf("[");
        for (int j = 0; j < size; j++)
        {
            printf("%.2f,\t", (&array)[i][j]);
        }
        printf("]\n");
    }
}