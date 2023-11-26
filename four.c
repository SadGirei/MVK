#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef SUM
    #define SIGN '+'
    int operation(int a, int b)
    {
        return a + b;
    }
#elif MULT
    #define SIGN '*'
    int operation(int a, int b)
    {
        return a * b;
    }

#endif

int main(int argc, char** argv)
{
    int rank, commSize;
    MPI_Status status;
    int count;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    if (rank == 0)
    {
        int x[] = {1, 2 ,3, 4, 5, 6, 7, 8, 9, 10};
        int* xptr = x;
        int y[] = {11, 12 ,13, 14, 15, 16, 17, 18, 19, 20};
        int* yptr = y;
        int dataNeedToSend = sizeof(x) / sizeof(x[0]);
        int dataForProcess;
        int leftProcess = commSize - 1;
        for(int i = 1; i < commSize; ++i)
        {
            if (dataNeedToSend % leftProcess != 0)
                dataForProcess = dataNeedToSend / leftProcess + 1; 
            else
                dataForProcess = dataNeedToSend / leftProcess;
            dataNeedToSend -= dataForProcess;
            leftProcess -= 1;
            MPI_Send(xptr, dataForProcess, MPI_INTEGER, i, 777, MPI_COMM_WORLD);
            MPI_Send(yptr, dataForProcess, MPI_INTEGER, i, 666, MPI_COMM_WORLD);
            xptr += dataForProcess;
            yptr += dataForProcess;
        }
        for(int i = 1; i < commSize; ++i)
        {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INTEGER, &count);
            int result[count];
            MPI_Recv(result, count, MPI_INTEGER, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
            printf("Получено сообщение от процесса %d: ", status.MPI_SOURCE);
            for (int i = 0; i < count; ++i)
            {
                printf("%d ", result[i]);
            }
            printf("\n");
        }
    }
    if (rank != 0)
    {
        MPI_Probe(0, 777, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INTEGER, &count);
        int xRecv[count];
        int yRecv[count];
        int z[count];
        MPI_Recv(xRecv, count, MPI_INTEGER, 0, 777, MPI_COMM_WORLD, &status);
        MPI_Recv(yRecv, count, MPI_INTEGER, 0, 666, MPI_COMM_WORLD, &status);
        for (int i = 0; i < count; ++i)
        {
            z[i] = operation(xRecv[i], yRecv[i]);
            printf("Результат процесса %d : %d %c %d = %d\n", rank, xRecv[i], SIGN, yRecv[i], z[i]);
        }
        MPI_Send(z, count, MPI_INTEGER, 0, 777, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}