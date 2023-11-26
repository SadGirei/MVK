// #include <mpi.h>
// #include <stdio.h>
// int main(int argc, char** argv)
// {
//     int rank;
//     int commSize;
//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &commSize);
//     int n = 5;
//     int x[n];
//     int dataPerProcessor[commSize];
//     int displacement[commSize];
//     int y[n / commSize + 1];
//     for(int i = 0; i < n; ++i)
//     {
//         y[i] = 0;
//     }
//     int xNorm[n / commSize + 1];
//     if (rank == 0)
//     {
//         //заполняем массив данных
//         for (int i = 0; i < n; ++i)
//         {
//             x[i] = i;
//         }
//         //смещение для текущего процессора
//         int temp = 0;
//         //заполняем сколько данных должен поллучить каждый процессор
//         for (int i = 0; i < commSize; ++i)
//         {
//             dataPerProcessor[i] = n / commSize + (n % commSize > i);
//             displacement[i] = temp;
//             temp += dataPerProcessor[i];
//         }
//     }
//     MPI_Scatterv(x, dataPerProcessor, displacement, MPI_INTEGER, y, n / commSize + 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
//     MPI_Reduce(y,xNorm,n / commSize + 1,MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
//     if(rank == 0)
//     {
//         int sum = 0;
//         for (int i = 0; i < n / commSize + 1; ++i)
//         {
//             sum += xNorm[i];
//         }
//         printf("%d\n",sum);
//     }


//     MPI_Finalize();
// }

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    int x[] = {99, 1, 2, 3};
    int n = sizeof(x) / sizeof(int);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //опеределяем сколько данных для каждого проца
    int elemPerRank = n / commSize + (n % commSize != 0);
    int *resizedX;
    if (rank == 0)
    {
        //дополняем массив чтобы был кратены commSize
        resizedX = resizeArray(x, n, elemPerRank * commSize);
    } 
    int xNorm;
    int a[elemPerRank];
    int xNormCurrentRank = 0;
    MPI_Scatter(resizedX, elemPerRank, MPI_INTEGER,a, elemPerRank, MPI_INTEGER, 0, MPI_COMM_WORLD);
    for (int i = 0; i < elemPerRank; ++i)
    {
        xNormCurrentRank += abs(a[i]);
    }
    MPI_Reduce(&xNormCurrentRank, &xNorm, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0)
        printf("%d\n", xNorm);
    MPI_Finalize();
}