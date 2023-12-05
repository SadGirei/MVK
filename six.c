#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define ARRAY_ID 777
#define n 1000000
#define times 10000
#ifdef PINGPONG
int main(int argc, char **argv)
{
    int commSize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    if (rank == 0)
    {
        int a[n];
        for(int i = 0; i < n; ++i)
        {
            a[i] = rand();
        }
        double start, end;
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for(int i = 0; i < times; ++i)
        {
            MPI_Send(a, n, MPI_INTEGER, 1, ARRAY_ID, MPI_COMM_WORLD);
            MPI_Recv(a, n, MPI_INTEGER, 1, ARRAY_ID, MPI_COMM_WORLD, &status);
        }
        end = MPI_Wtime();
        printf("%lf\n",(double)(end - start) );
    }
    if (rank == 1)
    {
        int b[n];
        MPI_Barrier(MPI_COMM_WORLD);
        for(int i = 0; i < times; ++i)
        {
            MPI_Recv(b, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD, &status);
            MPI_Send(b, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}
#elif PINGPING
int main(int argc, char **argv)
{
    int commSize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end;
    int a[n];
    int bufSize = n * sizeof(int) + MPI_BSEND_OVERHEAD;
    int *buf = malloc(bufSize);
    if(buf == 0)
    {
        printf("не удалось выделить столько места");
        return 0;
    }
    MPI_Buffer_attach(buf, bufSize);
    for(int i = 0; i < n; ++i)
    {
        a[i] = rand();
    }
    if (rank == 0)
    {   
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for(int i = 0; i < times; ++i)
        {
            MPI_Bsend(a, n, MPI_INTEGER, 1, ARRAY_ID, MPI_COMM_WORLD);
            MPI_Recv(a, n, MPI_INTEGER, 1, ARRAY_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        end = MPI_Wtime();
        printf("%lf\n",(double)(end - start) );
    }
    if (rank == 1)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for(int i = 0; i < times; ++i)
        {
            MPI_Bsend(a, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD);
            MPI_Recv(a, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        end = MPI_Wtime();
        printf("%lf\n",(double)(end - start));
    }
    MPI_Finalize();
}
#endif
