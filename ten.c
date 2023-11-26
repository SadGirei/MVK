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
    int A[3][3] = {{2, 0, 6}, {8, 1, -4}, {0, 5, 7}};
    int x[] = {2, 5, -3};
    int n = sizeof(x) / sizeof(int);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //опеределяем сколько данных для каждого проца
    int elemPerRank = n / commSize + (n % commSize != 0);
    int *resizedX;
    if (rank == 0)
    {
        //дополняем массивы чтобы были кратены commSize
        resizedX = resizeArray(x, n, elemPerRank * commSize);
    }
    int result;
    int a[elemPerRank];
    int multAB = 0;
    MPI_Bcast(x, n, MPI_INTEGER, 0, MPI_COMM_WORLD)
    MPI_Scatter(resizedX, elemPerRank, MPI_INTEGER,a, elemPerRank, MPI_INTEGER, 0, MPI_COMM_WORLD);
    for (int i = 0; i < elemPerRank; ++i)
    {
        multAB += a[i] * b[i];
    }
    MPI_Reduce(&multAB, &result, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0)
        printf("%d\n", result);
    MPI_Finalize();
}