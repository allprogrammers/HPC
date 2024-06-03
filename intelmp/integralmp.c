#include <stdio.h>
#include "omp.h"

#define NUM_STEPS 100000000

double evalFunction(double x)
{
    double answer = 4/(1+x*x);
    return answer;
}

int main (void)
{
    double start = 0;
    double end = 1;
    double step_size = (end-start)/NUM_STEPS;
    double integralans = 0;
    int actual_threads_num_outside = 0;
    
    double time = omp_get_wtime();
    #pragma omp parallel
    {
        int threadID = omp_get_thread_num();
        int actual_threads_num = omp_get_num_threads();
        if(!threadID)
        {
            actual_threads_num_outside = actual_threads_num;
        }
        int perthread = NUM_STEPS/actual_threads_num;
        
        int startx = threadID*perthread;
        int endx = startx + perthread;
        if(threadID==actual_threads_num-1)
        {
            endx = NUM_STEPS;
        }
        double accumulator = 0;
        
        for(int current=startx;current<endx;current++)
        {
            double height = evalFunction(current*step_size);
            accumulator += step_size*height;
        }
        
        #pragma omp atomic
        integralans += accumulator;
    }

    printf("Threads used: %d\n",actual_threads_num_outside);
    
    time = omp_get_wtime()-time;
    printf("time it took was %f\n", time);

    printf("the integral is %f\n",integralans);

    
    return 0;
}