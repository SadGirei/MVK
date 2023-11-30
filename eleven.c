#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EVEN_ID 777
#define ODD_ID 888
int main(int argc, char **argv)
{
    int commSize, rank;
    //определяем массивы
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    if (rank == 0)
    {
        int countT = 4;
        int blocklenT = 8;
        MPI_Datatype myVector;
        MPI_Type_vector(countT, blocklenT, 2 * blocklenT, MPI_INTEGER, &myVector);
        MPI_Type_commit(&myVector);

        int a[8][8] =
            {
                {0, 0, 0, 0, 0, 0, 0, 0}, 
                {1, 1, 1, 1, 1, 1, 1, 1}, 
                {2, 2, 2, 2, 2, 2, 2, 2}, 
                {3, 3, 3, 3, 3, 3, 3, 3}, 
                {4, 4, 4, 4, 4, 4, 4, 4}, 
                {5, 5, 5, 5, 5, 5, 5, 5}, 
                {6, 6, 6, 6, 6, 6, 6, 6}, 
                {7, 7, 7, 7, 7, 7, 7, 7} 
            };
        MPI_Send(a, 1, myVector, 1, EVEN_ID, MPI_COMM_WORLD);
        MPI_Send(&(a[1][0]), 1, myVector, 1, ODD_ID, MPI_COMM_WORLD);
    }
    if (rank == 1)
    {
        int b[4][8];
        MPI_Recv(b, 4 * 8, MPI_INTEGER, 0, EVEN_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("b=\n");
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
        int c[4][8];
        MPI_Recv(c, 4 * 8, MPI_INTEGER, 0, ODD_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("c=\n");
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
        
    }
    MPI_Finalize();
}