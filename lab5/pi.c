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
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int i=10;i<=ITERATIONS; i=i*10)
            {
                #pragma omp task
                {
                    int outside = 0;
                    int inside = 0;
                    #pragma omp parallel for reduction (+:inside)
                    for(int j = 0;j<i;j++)
                    {
                        generate_points(centerx, centery, radius, &inside);
                    }

                    double estimate = 4.0*(inside)/i;

                    printf("estimate %d %f\nerror %f\n",inside+outside, estimate, estimate-PI);
                    printf("done %d\n",i);
                }
            }
        }
    }
    

    return 0;

}