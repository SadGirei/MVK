#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define ARRAY_ID 777
#define ARRAY_INDEX_ID 778
#define ARRAY_M_ID 779
#define ARRAY_X_ID 780
typedef struct
{
    int* array;
    int index;
} Array;
int main(int argc, char **argv)
{
    int commSize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        //Задаем массивы A x
        int commRecivers = commSize - 1;
        int A[][4] = {{1, 2, 3, 0}, {4, 5, 6, 0}, {7, 8, 0, 9}, {666, 777, 888, 999}};
        int n = sqrt(sizeof(A) / sizeof(int));
        int x[n];
        //задаем массив для количества массивов на отправку в один процесс
        int arrayPerReciever[commSize];
        for (int i = 1; i < commSize; ++i)
        {
            arrayPerReciever[i] = n / commRecivers + (n % commRecivers >= i);
        }
        //сколько уже отправлено данных
        int sendedData = 0;
        //отправляем данные на каждый процессор
        //1 сначало ссылку на начало массива
        //2 смещение относительно начала
        //3 сколько массивов отправлено в один процессор
        for(int i = 1; i < commSize; ++i)
        {
            MPI_Send(A[sendedData], n * arrayPerReciever[i], MPI_INTEGER, i, ARRAY_ID, MPI_COMM_WORLD);
            MPI_Send(&sendedData, 1, MPI_INTEGER, i, ARRAY_INDEX_ID, MPI_COMM_WORLD);
            MPI_Send(&arrayPerReciever[i], 1, MPI_INTEGER, i, ARRAY_M_ID, MPI_COMM_WORLD);
            sendedData += arrayPerReciever[i];
        }       
        //сколько уже полученно данных
        int receivedData = 0;
        MPI_Status status;
        //получаем данные из каждого процессора
        for (int i = 1; i < commSize; ++i)
        {
            MPI_Recv(x + receivedData, arrayPerReciever[i], MPI_INTEGER, i, ARRAY_X_ID, MPI_COMM_WORLD, &status);
            receivedData += arrayPerReciever[i];
        }
        //выводим
        for (int i = 0; i < n; ++i)
        {
            printf("%d ", x[i]);
        }
    }
    if (rank != 0)
    {
        MPI_Status status;
        int count;
        MPI_Probe(0, ARRAY_ID, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INTEGER, &count);
        int b[count];
        int index;
        int m;
        MPI_Recv(b, count, MPI_INTEGER, 0, ARRAY_ID, MPI_COMM_WORLD, &status);
        MPI_Recv(&index, 1, MPI_INTEGER, 0, ARRAY_INDEX_ID, MPI_COMM_WORLD, &status);
        MPI_Recv(&m, 1, MPI_INTEGER, 0, ARRAY_M_ID, MPI_COMM_WORLD, &status);
        int x[m];
        for(int i = 0; i < m; ++i)
        {
            for(int j = 0; j < count / m; ++j)
            {
                if(index == j)
                    x[i] = b[j + i * count / m];
            }
            ++index;
        }
        MPI_Send(x, m, MPI_INTEGER, 0, ARRAY_X_ID, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
}