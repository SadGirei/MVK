#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
typedef struct
{
    int firstRow[8];
    int secondRow[8];
} TwoRowStruct;

int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Datatype twoRowStructDataType;
    MPI_Datatype types[2] = {MPI_INTEGER, MPI_INTEGER};
    int blocklens[2] = {8, 8};
    MPI_Aint displs[2];
    displs[0] = offsetof(TwoRowStruct, firstRow);
    displs[1] = offsetof(TwoRowStruct, secondRow);
    MPI_Type_create_struct(2, blocklens, displs, types, &twoRowStructDataType);
    MPI_Type_commit(&twoRowStructDataType);
    if (rank == 0)
    {
        int a[8][8] =
            {
                {0, 0, 0, 0, 0, 0, 0, 0}, 
                {1, 1, 1, 1, 1, 1, 1, 1}, 
                {2, 2, 2, 2, 2, 2, 2, 2}, 
                {3, 3, 3, 3, 3, 3, 3, 3}, 
                {4, 4, 4, 4, 4, 4, 4, 4}, 
                {5, 5, 5, 5, 5, 5, 5, 5}, 
                {6, 6, 6, 6, 6, 6, 6, 6}, 
                {7, 7, 7, 7, 7, 7, 7, 7} 
            };
        TwoRowStruct twoRowStructs[4];
        for (int i = 0; i < 4; ++i)
        {
            memcpy(twoRowStructs[i].firstRow, a[i],8 * sizeof(int));
            memcpy(twoRowStructs[i].secondRow, a[4 + i],8 * sizeof(int));
            MPI_Send(&twoRowStructs[i], 1, twoRowStructDataType, i, 777, MPI_COMM_WORLD); 
        }
        // MPI_Send(&(twoRowStructs[1]), 1, twoRowStructDataType, 2, 777, MPI_COMM_WORLD);
        // MPI_Send(&(twoRowStructs[2]), 1, twoRowStructDataType, 3, 777, MPI_COMM_WORLD);
        // MPI_Send(&(twoRowStructs[3]), 1, twoRowStructDataType, 4, 777, MPI_COMM_WORLD);
    }
    int d[2][8];
    MPI_Recv(d, 2 * 8, twoRowStructDataType, 0, 777, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("rank = %d, d = \n", rank);
    for(int i = 0; i < 2; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            printf("%d ", d[i][j]);
        }
        printf("\n");
    }
    MPI_Finalize();
}