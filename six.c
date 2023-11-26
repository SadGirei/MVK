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
    if (rank == 0)
    {
        MPI_Status status;
        double start, end;
        int a[n];
        for(int i = 0; i < n; ++i)
        {
            a[i] = rand();
        }
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
        MPI_Request request;
        MPI_Status status;
        double start, end;
        int b[n];
        for(int i = 0; i < n; ++i)
        {
            b[i] = rand();
        }
        start = MPI_Wtime();
        for(int i = 0; i < times; ++i)
        {
            MPI_Irecv(b, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD, &request);
            MPI_Send(b, n, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD);
            MPI_Wait(&request, &status);
        }
        end = MPI_Wtime();
        printf("%lf\n",(double)(end - start));
    }
    MPI_Finalize();
}
#endif
