#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 100

void addVectors(int vector1[], int vector2[], int result[], int length)
{
	for(int i=0; i<length;i++)
    {
        result[i] = 2*vector1[i]+9*vector2[i];
	}
}

int main()
{
    FILE *file;

    int vector1[MAX_ELEMENTS], vector2[MAX_ELEMENTS], result[MAX_ELEMENTS];
    char *filename = "vectors.txt";

    char *env_elements_str = getenv("NUM_ELEMENTS");
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

    file = fopen(filename, "r");

    if(file==NULL)
    {
        printf("Error Opening the file.\n");
        return 1;
    }

    for(int i = 0; i<num_elements;i++)
    {
        if (fscanf(file, "%d %d", &vector1[i], &vector2[i]) != 2)
        {
            printf("Error reading from the file. \n");
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    addVectors(vector1, vector2, result, num_elements);
    printf("The result of the vector addition:\n");
    for(int i = 0;i<num_elements;i++)
    {
        printf("%d \n", result[i]);
    }

    printf("\n");
    return 0;

}
