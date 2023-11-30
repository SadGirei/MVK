#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    //создвние типа верхней треугольной матрицы
    int n = 8;
    int blocklens[n];
    int displs[n];
    MPI_Datatype UpperMatrix;
    for (int i = 0; i < n; ++i)
    {
        blocklens[i] = n - i;
        displs[i] = n * i + i;
    }
    MPI_Type_indexed(n, blocklens, displs, MPI_INTEGER, &UpperMatrix);
    MPI_Type_commit(&UpperMatrix);
    int a[8][8];
    if (rank == 0)
    {
        int a[8][8] =
            {
                {1, 1, 1, 1, 1, 1, 1, 1}, 
                {0, 2, 2, 2, 2, 2, 2, 2}, 
                {0, 0, 3, 3, 3, 3, 3, 3}, 
                {0, 0, 0, 4, 4, 4, 4, 4}, 
                {0, 0, 0, 0, 5, 5, 5, 5}, 
                {0, 0, 0, 0, 0, 6, 6, 6}, 
                {0, 0, 0, 0, 0, 0, 7, 7}, 
                {0, 0, 0, 0, 0, 0, 0, 8} 
            };
    }
    MPI_Bcast(a, 1, UpperMatrix, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            if (j >= i)
                printf("%d ", a[i][j]);
            else
                printf("  ");
        printf("\n");
    }
    MPI_Finalize();
}