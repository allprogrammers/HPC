#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

#define MAX_ELEMENTS 1000000

void avgVectors(float A, int vector1[], int vector2[], float result[], int length)
{
    double time = omp_get_wtime();
    #pragma omp parallel for
	for(int i = 0; i < length; i++)
    {
        result[i] = A*vector1[i]+vector2[i];
	}
    time = omp_get_wtime()-time;

    printf("It took %f\n", time);
}

int readFile(char* filename,int num_elements,int vector[])
{
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

int main(void)
{
    
    int* vector1 = (int*) malloc(MAX_ELEMENTS*sizeof(int));
    
    int* vector2 = (int*) malloc(MAX_ELEMENTS*sizeof(int));
    printf("memory allocated for two ints\n");
    float* result = (float*) malloc(MAX_ELEMENTS*sizeof(float));;
    printf("memory allocated for float\n");
    char* file1name = "vector_x.txt";
    char* file2name = "vector_y.txt";
    

    char* env_elements_str = getenv("NUM_ELEMENTS");
    int num_elements;

    if(env_elements_str == NULL)
    {

        printf("Environment variable NUM_ELEMENTS is noto set.\n");

        return 1;

    }

    sscanf(env_elements_str, "%d", &num_elements);

    if(num_elements <= 0 || num_elements > MAX_ELEMENTS)
    {
        printf("Invalid number of elements from environmental variable. \n");
        return 1;
    }

    float A = -2.56;
    if (num_elements>7500)
    {
        A = 102.55;
    }else if(num_elements>5000)
    {
        A = -59.2;
    }else if(num_elements>2000)
    {
        A = 25.69;
    }
    
    {
        int returnval = readFile(file1name,num_elements,vector1);
        if(returnval)
        {
            return returnval;
        }

        returnval = readFile(file2name,num_elements,vector2);
        if(returnval)
        {
            return returnval;
        }
    }

    avgVectors(A,vector1, vector2, result, num_elements);
    //printf("The result of the vector addition:\n");

    FILE* file_avg = fopen("vector_z.txt","w");

    
    for(int i = 0; i<num_elements;i++)
    {
        fprintf(file_avg, "%d %d %.2f \n", vector1[i], vector2[i],result[i]);
    }
    
    fclose(file_avg);

    /*for(int i = 0;i<num_elements;i++)
    {
        printf("%f \n", result[i]);
    }*/

    printf("\n");
    return 0;

}
