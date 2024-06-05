#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 100000

void addVectors(int vector1[], int vector2[], int result[], int length)
{
    
    
    for(int i = 0; i <= length; i++)
    {
        result[i] = vector1[i]+vector2[i];
    }
}

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

int main(void)
{

    int* vector1 = (int*) malloc(MAX_ELEMENTS*sizeof(int));
    int* vector2 = (int*) malloc(MAX_ELEMENTS*sizeof(int));

    int* vector[2] = {vector1,vector2};
    printf("memory allocated for two ints\n");

    int* result = (int*) malloc(MAX_ELEMENTS*sizeof(int));;
    printf("memory allocated for results\n");
    
    char* filename[2] = {"vector_x.txt","vector_y.txt"};
    

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


    for(int i = 0;i<2;i++)
    {
        readFile(filename[i],num_elements,vector[i]);
    }

    addVectors(vector[0], vector[1], result, num_elements);
    //printf("The result of the vector addition:\n");

    FILE* file_avg = fopen("vector_z_tester.txt","w");

    
    for(int i = 0; i<num_elements;i++)
    {
        fprintf(file_avg, "%d %d %d \n", vector[0][i], vector[1][i],result[i]);
    }
    
    fclose(file_avg);
    free(vector[0]);
    free(vector[1]);
    free(result);

    /*for(int i = 0;i<num_elements;i++)
    {
        printf("%f \n", result[i]);
    }*/

    printf("\n");
    return 0;

}
