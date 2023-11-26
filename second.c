#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int a[10];
        srand(time(NULL));
        for (int i = 0; i < 10; ++i)
        {
            a[i] = rand();
            printf("a[%d] = %d\n",i,a[i]);
        }
        MPI_Send(a,10,MPI_INTEGER,1,5,MPI_COMM_WORLD);
    }
    printf("----------------\n");
    if (rank == 1)
    {
        int b[10];
        MPI_Status status;
        MPI_Recv(b, 10, MPI_INTEGER, 0, 5, MPI_COMM_WORLD, &status);
        for (int i = 0; i < 10; ++i)
        {
            printf("b[%d] = %d\n",i,b[i]);
        }
    }
    MPI_Finalize();
}