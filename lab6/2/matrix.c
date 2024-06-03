#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NRA 1000
#define NCA 1000
#define NCB 1000

typedef struct Matrix{
    int rows;
    int columns;
    double* matrix;
} Matrix;

double matrix_getter(Matrix matrix, int row, int column)
{
    return *(matrix.matrix+row*matrix.columns+column);
}

void matrix_setter(Matrix matrix, int row, int column,double element)
{
    *(matrix.matrix+row*matrix.columns+column)=element;
}

void printMatrix(Matrix matrix)
{
    for(int i=0;i<matrix.rows;i++)
    {
        for(int j=0;j<matrix.columns;j++)
        {
            //printf("the number is inside\n");
            printf("%.1f,\t",matrix_getter(matrix,i,j));
            //printf("finished column of one row\n");
        }
        printf("\n");
    }
}

void initialize_matrix(Matrix matrix)
{

    static int count = 0;
    //#pragma omp parallel for
    for(int i=0;i<matrix.rows;i++)
    {
        //#pragma omp parallel for
        for(int j=0;j<matrix.columns;j++)
        {
            //printf("the number is inside\n");
            matrix_setter(matrix,i,j,i+j);
            //printf("finished column of one row\n");
        }
        //printf("finished one row\n");
    }
    
}


double dot( Matrix matrixA, int rowA,
            Matrix matrixB, int columnB)
{
    if(matrixA.columns!=matrixB.rows)
    {
        printf("error in dot\n");
        exit(1);
    }

    double result = 0;

    //removing this pragma helps a lot
    #pragma omp parallel for
    for (int i=0;i<matrixA.columns;i++)
    {
        result += matrix_getter(matrixA,rowA,i)*matrix_getter(matrixB,columnB,i);
        //result += matrixA[rowA][i]*matrixB[i][columnB];
    }
    return result;
}

void multiply_matrix(   Matrix matrixA,
                        Matrix matrixB,
                        Matrix matrixC)
{
    int total = matrixC.rows*matrixC.columns;
    omp_set_schedule(omp_sched_static, matrixC.columns);
    #pragma omp parallel for
    for(int k=0;k<total;k++)
    {
        int i = k/matrixC.columns;
        int j = k%matrixC.columns;
        matrix_setter(matrixC, i, j, dot(matrixA, i, matrixB, j));
    }
}

int main(void)
{
    Matrix A;
    

    Matrix B;
    

    Matrix C;
    

    printf("finished declaring\n");

    double timeprog = omp_get_wtime();
    //starting with 1000, 1000, 1000, time 5.837667
    //with only sections you get 5.743607
    // with omp parallel for x3 you get 2.367832
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            A.rows = NRA;
            A.columns = NCA;
            A.matrix = (double*) malloc(A.rows*A.columns*sizeof(double));
            initialize_matrix(A);
            //printMatrix(A);
            printf("finished initializing A\n");
        }

        #pragma omp section
        {
            B.rows = NCB;
            B.columns = NCA;
            B.matrix = (double*) malloc(B.rows*B.columns*sizeof(double));
            initialize_matrix(B);
            //printMatrix(B);
            printf("finished initializing B\n");
        }

        #pragma omp section
        {
            C.rows = NRA;
            C.columns = NCB;
            C.matrix = (double*) malloc(C.rows*C.columns*sizeof(double));
            //printMatrix(C);
            printf("finished initializing C\n");
        }
    }
    
    
    multiply_matrix(A, B, C);
    printf("finished multiplying\n");

    //printMatrix(C);

    timeprog = omp_get_wtime()-timeprog;

    printf("it took %f\n",timeprog);
    

    free(A.matrix);
    free(B.matrix);
    free(C.matrix);
    
    return 0;
}