#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iostream>

double getRandomNumber(){
    return rand() % (int)pow(10,7)+ 1;
}

int main(int argc, char *argv[])
{
    int matrixSize = atoi(argv[1]);

    srand(time(NULL));

    std::ofstream csvFile;

    csvFile.open("outside_i_time.csv", std::ios::app);

    double **A = (double **)malloc(matrixSize * sizeof(double *));
    double *x = (double *)malloc(matrixSize * sizeof(double));
    double *B = (double *)malloc(matrixSize * sizeof(double));
    double aux = 0;

    double time_spent = 0.0;

    clock_t begin = clock();

    for (int i = 0; i < matrixSize; i++)
    {
        A[i] = (double *)malloc(matrixSize * sizeof(double));
        x[i] = getRandomNumber();
        for (int j = 0; j < matrixSize; j++)
        {
            A[i][j] = getRandomNumber();
        }
    }

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            B[i] += A[i][j] * x[j];
        }
    }
    for (int i = 0; i < matrixSize; i++)
    {
        free(A[i]);
    }
    free(x);
    free(B);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    csvFile << matrixSize << "," << time_spent << ",";
    csvFile.close();
    return 0;
}
