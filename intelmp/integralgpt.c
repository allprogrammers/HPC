#include <stdio.h>
#include <omp.h>

#define NUM_STEPS 100000000

double evalFunction(double x)
{
    return 4.0 / (1.0 + x * x);
}

int main(void)
{
    double start = 0.0;
    double end = 1.0;
    double step_size = (end - start) / NUM_STEPS;
    double integralans = 0.0;
    int max_threads = omp_get_max_threads();
    
    double time = omp_get_wtime();
    
    #pragma omp parallel
    {
        int threadID = omp_get_thread_num();
        int actual_threads_num = omp_get_num_threads();
        double thread_integral = 0.0;
        
        int per_thread = NUM_STEPS / actual_threads_num;
        int start_index = threadID * per_thread;
        int end_index = (threadID == actual_threads_num - 1) ? NUM_STEPS : start_index + per_thread;

        for (int i = start_index; i < end_index; i++)
        {
            double x = (i + 0.5) * step_size;
            thread_integral += evalFunction(x);
        }

        thread_integral *= step_size;

        #pragma omp atomic
        integralans += thread_integral;
    }

    time = omp_get_wtime() - time;

    printf("Threads used: %d\n", max_threads);
    printf("Time taken: %f seconds\n", time);
    printf("The integral approximation of pi is %f\n", integralans);

    return 0;
}
