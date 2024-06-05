#include <stdio.h>
#include <omp.h>
// CUDA kernel to add two vectors

#define MAX_ELEMENTS 100000

__global__ void vectorAdd(int* a, int* b, int* c, int n)
{
    // Get the global thread ID
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    // Check if the thread ID is within the vector size
    if (tid<n)
    {
        c[tid] = a[tid] + b[tid];
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

void writeFile(int num_elements,int* vector1, int* vector2, int* result)
{
    FILE* file_avg = fopen("vector_z.txt","w");
    
    for(int i = 0; i<num_elements;i++)
    {
        fprintf(file_avg, "%d %d %d \n", vector1[i], vector2[i],result[i]);
    }
    
    fclose(file_avg);
}

int main(void)
{
    char* env_elements_str = getenv("NUM_ELEMENTS");
    int n;

    if(env_elements_str == NULL)
    {

        printf("Environment variable NUM_ELEMENTS is noto set.\n");

        return 1;

    }

    sscanf(env_elements_str, "%d", &n);

    if(n <= 0 || n > MAX_ELEMENTS)
    {
        printf("Invalid number of elements from environmental variable. \n");
        return 1;
    }

    

    int* a; // Host vectors
    int* b;
    int* c;
    int* d_a; //Device vectors
    int* d_b;
    int* d_c;

    // Allocate memory for host vectors
    a = (int*) malloc(n * sizeof(int));
    b = (int*) malloc(n * sizeof(int));
    c = (int*) malloc(n * sizeof(int));

    readFile("vector_x.txt",n,a);
    readFile("vector_y.txt",n,b);

    
    // Initialize host vectors
    /*for (int i = 0; i < n; i++)
    {
        a[i] = i;
        b[i] = i;
    }*/
    n=n/10;

    omp_set_schedule(omp_sched_auto, 1);
    #pragma omp parallel for
    for(int i = 0;i<10;i++)
    {
        printf("%d\n", omp_get_num_threads());
        cudaEvent_t start, stop;
        float elapsedTime;
        cudaEventCreate(&start); // create event objects
        cudaEventCreate(&stop);
        cudaEventRecord(start, 0);

        // Allocate memeory for device vectors
        cudaMalloc(&d_a,n*sizeof(int));
        cudaMalloc(&d_b,n*sizeof(int));
        cudaMalloc(&d_c,n*sizeof(int));
        
        // copy host vectors to device

        cudaMemcpy(d_a, (a+i*n), n * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, (b+i*n), n * sizeof(int), cudaMemcpyHostToDevice);

        // Define grid and block dimensions

        int blockSize = 256;
        int gridSize = (n+blockSize -1)/blockSize;

        

        // Launch the vectorAdd CUDA kernel

        vectorAdd<<<gridSize, blockSize>>>(d_a,d_b,d_c,n);

        // Copy result from device to host

        cudaMemcpy((c+i*n), d_c, n * sizeof(int), cudaMemcpyDeviceToHost);

        cudaEventRecord(stop, 0); // record end event
        cudaEventSynchronize(stop); // wait for all device work to complete
        cudaEventElapsedTime(&elapsedTime, start, stop); //time between events
        cudaEventDestroy(start); //destroy start event
        cudaEventDestroy(stop);

        printf("time elapsed: %f\nthread %d\n", elapsedTime,i);

    }
    
    // Verify the result

    n=n*10;
    for(int i=0;i<n;i++)
    {
        if(c[i] != a[i]+b[i])
        {
            printf("Error: Result verification failed at element %d!\n",i);
            return 1;
        }
    }

    writeFile(n,a,b,c);

    printf("Vector addition completed successfully!\n");

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    // Free host memory
    
    free(a);
    free(b);
    free(c);
    
    return 0;
}