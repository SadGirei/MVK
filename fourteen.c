#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INDEX 777
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    int a[10];
    double b[8];
    int byteCount;
    int dataSize = 0;
    MPI_Pack_size(10, MPI_INTEGER, MPI_COMM_WORLD, &byteCount);
    dataSize += byteCount;
    MPI_Pack_size(8, MPI_DOUBLE, MPI_COMM_WORLD, &byteCount);
    dataSize += byteCount;
    char packedData[dataSize];
    int position = 0;
    if(rank == 0)
    {
        int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        double b[8] = {0.1, 0.02, 0.003, 0.0004,
                    0.0005, 0.00006, 0.000007, 0.000008};
        MPI_Pack(a, 10, MPI_INTEGER, packedData, dataSize, &position, MPI_COMM_WORLD);
        MPI_Pack(b, 8, MPI_DOUBLE, packedData, dataSize, &position, MPI_COMM_WORLD);
        position = 0;
    }
    MPI_Bcast(packedData, dataSize, MPI_PACKED, 0, MPI_COMM_WORLD);
    MPI_Unpack(packedData, dataSize, &position, a, 10, MPI_INTEGER, MPI_COMM_WORLD);
    // printf("%d", dataSize);
    MPI_Unpack(packedData, dataSize, &position, b, 8, MPI_DOUBLE, MPI_COMM_WORLD);
    printf("Процеесор: %d. \na = ", rank);
    for(int i = 0; i < 10; ++i)
        printf("%d ", a[i]);
    printf("\nb = \n");
    for(int i = 0; i < 8; ++i)
        printf("%f ", b[i]);
    printf("\n");
    MPI_Finalize();
}