#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_DOUBLE_INT;
    printf("hello");
    MPI_Finalize();
}