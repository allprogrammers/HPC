#include <stdio.h>

// CUDA kernel to add two vectors

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

int main(void)
{
    int n = 10000; //Vector size
    int* a; // Host cectors
    int* b;
    int* c;
    int* d_a; //Device vectors
    int* d_b;
    int* d_c;

    // Allocate memory for host vectors
    a = (int*) malloc(n * sizeof(int));
    b = (int*) malloc(n * sizeof(int));
    c = (int*) malloc(n * sizeof(int));
    
    // Initialize host vectors
    for (int i = 0; i < n; i++)
    {
        a[i] = i;
        b[i] = i;
    }

    // Allocate memeory for device vectors
    cudaMalloc(&d_a,n*sizeof(int));
    cudaMalloc(&d_b,n*sizeof(int));
    cudaMalloc(&d_c,n*sizeof(int));
    
    // copy host vectors to device

    cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(int), cudaMemcpyHostToDevice);

    // Define grid and block dimensions

    int blockSize = 256;
    int gridSize = (n+blockSize -1)/blockSize;

    // Launch the vectorAdd CUDA kernel

    vectorAdd<<<gridSize, blockSize>>>(d_a,d_b,d_c,n);

    // Copy result from device to host

    cudaMemcpy(c, d_c, n * sizeof(int), cudaMemcpyDeviceToHost);
    
    // Verify the result

    for(int i=0;i<n;i++)
    {
        if(c[i] != a[i]+b[i])
        {
            printf("Error: Result verification failed at element %d!\n",i);
            return 1;
        }
    }

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