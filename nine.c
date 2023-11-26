#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
typedef struct {
    double val;
    int index;
} DOUBLE_INT;
DOUBLE_INT* createResizedDoubleIntArray(const double* oldArray,int oldSize, int newSize)
{
    DOUBLE_INT *newArray = malloc(newSize * sizeof(DOUBLE_INT));
    for(int i = 0; i < newSize; ++i)
    {
        if (i < oldSize)
        {
            newArray[i].val = oldArray[i];
            newArray[i].index = i;
        }
        else
        {
            newArray[i].val = DBL_MAX;
            newArray[i].index = i;
        }
    }
    return newArray;
}
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    double x[] = {0.5, 1.7, 2, 1, 3.999, -1.232, -18, -999.12, -1821.12, 21,123, 13.23, -1000,1};
    int n = sizeof(x) / sizeof(double);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //опеределяем сколько данных для каждого проца
    int elemPerRank = n / commSize + (n % commSize != 0);
    DOUBLE_INT *resizedX;
    if (rank == 0)
    {
        //дополняем массивы чтобы были кратены commSize
        resizedX = createResizedDoubleIntArray(x, n, elemPerRank * commSize);
    }
    DOUBLE_INT result;
    DOUBLE_INT a[elemPerRank];
    MPI_Scatter(resizedX, elemPerRank, MPI_DOUBLE_INT,a, elemPerRank, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
    //ищем минимум на текущем процессоре
    DOUBLE_INT minOnCurrentRank = {DBL_MAX, -1};
    for (int i = 0; i < elemPerRank; ++i)
    {
        minOnCurrentRank = minOnCurrentRank.val < a[i].val ? minOnCurrentRank : a[i];
    }
    //ищем минимум на всех процессорах и отправляем на 0 процессор
    MPI_Reduce(&minOnCurrentRank, &result, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    if(rank == 0)
    {
        printf("%f\n", result.val);
        printf("%d\n", result.index);
    }
    MPI_Finalize();
}