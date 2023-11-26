#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int* resizeArray(const int* a,int oldSize, int newSize)
{
    int *newArray = calloc(newSize, sizeof(int));
    memcpy(newArray, a, sizeof(int) * oldSize);
    return newArray;
}
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    int x[] = {0, 1, 2, 3};
    int y[] = {1, 1, 2, 2};
    int n = sizeof(x) / sizeof(int);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //опеределяем сколько данных для каждого проца
    int elemPerRank = n / commSize + (n % commSize != 0);
    int *resizedX, *resizedY;
    if (rank == 0)
    {
        //дополняем массивы чтобы были кратены commSize
        resizedX = resizeArray(x, n, elemPerRank * commSize);
        resizedY = resizeArray(y, n, elemPerRank * commSize);
    }
    int result;
    int a[elemPerRank];
    int b[elemPerRank];
    int multAB = 0;
    MPI_Scatter(resizedX, elemPerRank, MPI_INTEGER,a, elemPerRank, MPI_INTEGER, 0, MPI_COMM_WORLD);
    MPI_Scatter(resizedY, elemPerRank, MPI_INTEGER,b, elemPerRank, MPI_INTEGER, 0, MPI_COMM_WORLD);
    for (int i = 0; i < elemPerRank; ++i)
    {
        multAB += a[i] * b[i];
    }
    MPI_Reduce(&multAB, &result, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0)
        printf("%d\n", result);
    MPI_Finalize();
}