#include <stdio.h>
#include <mpi.h>

#define MAX_ELEMENTS 1000000
#define TAG_DATA 1
#define TAG_MAX 2

int readFile(char* filename,int num_elements,int vector[])
{

    //each part of the file can be read by different thread (is it possible?)
    FILE* file = fopen(filename, "r");

    if(file==NULL)
    {
        printf("Error Opening the file.\n");
        return 1;
    }

    for(int i = 0; i<num_elements;i++)
    {
        if (fscanf(file, "%d", &vector[i]) != 1)
        {
            printf("Error reading from the file. \n");
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
/*
int find_max(int vector[], int length)
{
    if(length==0)
    {
        return 0;
    }
    int max = vector[0];
    for(int i=1;i<length;i++)
    {
        if(vector[i]>max)
        {
            max = vector[i];
        }
    }
    return max;

}

int function_rank_main(int rank, int size)
{

    int vector[MAX_ELEMENTS];
    readFile("vector_x.txt", MAX_ELEMENTS, vector);

    int sizeN = MAX_ELEMENTS/size;
    int remaining = MAX_ELEMENTS%size;
    #pragma omp parallel for
    for(int i=1;i<size;i++)
    {
        MPI_Send(vector+remaining+i*sizeN, sizeN, MPI_INT,i,TAG_DATA,MPI_COMM_WORLD);
    }
    
    
    remaining += MAX_ELEMENTS/size;
    
    int localmax = find_max(vector,remaining);

    //printf("sent\n");
    int tempmax;
    MPI_Status Stat;
    for(int i=1;i<size;i++)
    {
        MPI_Recv(&tempmax,1,MPI_INT,MPI_ANY_SOURCE,TAG_MAX,MPI_COMM_WORLD,&Stat);
        if(tempmax>localmax)
        {
            localmax = tempmax;
        }
    }

    return localmax;
}

void function_rank_other(int rank, int size)
{
    int vector[MAX_ELEMENTS];

    MPI_Status Stat;

    MPI_Recv(&vector,MAX_ELEMENTS/size,MPI_INT,0,TAG_DATA,MPI_COMM_WORLD,&Stat);
    
    int count;
    MPI_Get_count(&Stat, MPI_INT, &count);

    int localmax = find_max(vector, count);
    MPI_Send(&localmax,1,MPI_INT,0,TAG_MAX,MPI_COMM_WORLD);
}*/

int main(int argc, char** argv)
{

    int vector[MAX_ELEMENTS];
    readFile("vector_x.txt", MAX_ELEMENTS, vector);

    double timeprog = MPI_Wtime();

    int tempmax = vector[0];
    for(int i=1;i<MAX_ELEMENTS;i++)
    {
        if(vector[i]>tempmax)
        {
            tempmax = vector[i];
        }
    }

    timeprog = MPI_Wtime() - timeprog;

    printf("the max is %d\nit took %f seconds\n",tempmax,timeprog);

    return 0;
}