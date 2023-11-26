#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char** argv)
{
    int rank;
    int comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (rank == 1)
    {
        for (int i = 0; i < comm_size; ++i)
        {
            char a[50];
            sprintf(a, "hello process %d from process %d", i, rank);
            MPI_Send(a, strlen(a), MPI_CHARACTER, i, 777, MPI_COMM_WORLD);
        }
    }
    if (rank != 1)
    {
        MPI_Status status;
        int count;
        MPI_Probe(1, 777, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHARACTER, &count);
        char b[count];
        MPI_Recv(b, count, MPI_CHARACTER, 1, 777, MPI_COMM_WORLD, &status);
        printf("%s\n", b);
    }
    MPI_Finalize();
}