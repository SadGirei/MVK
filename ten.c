#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// void print2DMatrix(int columns, int rows, int **a)
// {
//     for (int i = 0; i < rows; ++i)
//     {
//         for (int j = 0; j < columns; ++j)
//         {
//             printf("%d ",a[i][j]);
//         }
//         printf("\n");
//     }
// }
// int** resize2DMatrix(int columnSize, int oldSizeRow, int newSizeRow, int (*a)[columnSize])
// {
//     int **matrix = malloc(newSizeRow * sizeof(int*) + columnSize * newSizeRow * sizeof(int));
//     int* numbers_start = (int *)(matrix + newSizeRow);
//     for (int i = 0; i < newSizeRow; i++) 
//     {
//         matrix[i] = numbers_start + i * columnSize;
//     }
//     memcpy(matrix[0], a, sizeof(int) * oldSizeRow * columnSize);
//     return matrix;
// }
int* resizeMatrix(int columnSize, int oldSizeRow, int newSizeRow, int (*a)[columnSize])
{
    int *matrix = calloc(columnSize * newSizeRow, sizeof(int));
    for (int i = 0; i < oldSizeRow; ++i)
    {
        for (int j = 0; j < columnSize; ++j)
            matrix[i * columnSize + j] = a[i][j];
    }
    return matrix;
}
void printMatrix(int columns, int rows, int *a)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            printf("%d ",a[i * columns + j]);
        }
        printf("\n");
    }
}
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    int A[][3] = {{2, 0, 6}, {8, 1, -4}, {0, 5, 7}};
    int x[] = {2, 5, -3};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //опеределяем сколько данных для каждого проца
    int n = sizeof(x) / sizeof(int);
    int elemPerRank = n / commSize + (n % commSize != 0);
    int *resizedMatrix = 0;
    if (rank == 0)
    {
        //делаем данные четно количеству процессоров
        resizedMatrix = resizeMatrix(n, n, elemPerRank * commSize, A);
    }
    //раскидываем x
    MPI_Bcast(x, n, MPI_INTEGER, 0, MPI_COMM_WORLD);
    //расскидываем A по всем процессорам
    int *a = malloc(n * elemPerRank * sizeof(int));
    MPI_Scatter(resizedMatrix, elemPerRank * n, MPI_INTEGER, a, elemPerRank * n, MPI_INTEGER, 0, MPI_COMM_WORLD);
    //произведение A * x
    int multAx[elemPerRank];
    for (int i = 0; i < elemPerRank; ++i)
    {
        multAx[i] = 0;
        for (int j = 0; j < n; ++j)
        {
            multAx[i] += a[i * n + j] * x[j];
        }
    }
    //объеденяем результаты
    int result[elemPerRank * commSize];
    MPI_Gather(multAx, elemPerRank, MPI_INTEGER, result, elemPerRank, MPI_INTEGER, 0, MPI_COMM_WORLD);
    //выводим ответ
    if (rank == 0)
    {
        for (int i = 0; i < n; ++i)
        {
            printf(" %d\n", result[i]);
        }
    }
    MPI_Finalize();
}