#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define ITERATIONS 10000000l


void generate_points(double centerx, double centery,double radius, int* inside)
{
    double x = rand();
    double y = rand();


    double rdistance = sqrt((x-centerx)*(x-centerx)+(y-centery)*(y-centery));


    //printf(" %d %d %f\n",x,y,rdistance);

    if(radius>=rdistance)
    {
        *inside += 1;
    }

}

int main (void)
{
    double PI = 3.141592653589793238462643;

    srand(time(NULL));

    double centerx = 0;
    double centery = 0;
    double radius = RAND_MAX;

    double timeprog = omp_get_wtime();
    int numiter[7] = {10,100,1000,10000,100000,1000000,10000000};

    #pragma omp parallel for
    for(int ix=0;ix<7; ix+=1)
    {
        double timeiter = omp_get_wtime();

        int i = numiter[ix];
        int outside = 0;
        int inside = 0;
        #pragma omp parallel for reduction (+:inside)
        for(int j = 0;j<i;j++)
        {
            generate_points(centerx, centery, radius, &inside);
            
            //printf("thread %d \n", omp_get_thread_num());

        }

        double estimate = 4.0*(inside)/i;
        timeiter = omp_get_wtime()-timeiter;

        printf("estimate %f %d taking %f\n",estimate, omp_get_thread_num(),timeiter);
        //printf("done %d\n",i);
        
    }
    timeprog = omp_get_wtime()-timeprog;

    printf("it took %f\n",timeprog);
    

    return 0;

}