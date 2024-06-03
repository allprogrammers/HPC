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
    double integral = 0;

    double time = omp_get_wtime();
    #pragma omp parallel for reduction (+:integral)
    for(int i=0;i<NUM_STEPS;i++)
    {
        double height = evalFunction(i*step_size);
        integral += step_size*height;
    }
    time = omp_get_wtime()-time;
    printf("time it took was %f\n", time);
    printf("the integral is %f \n",integral);
    return 0;
}