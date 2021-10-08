#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iostream>

double getRandomNumber(){
    srand(time(NULL));
    return rand() % (int)pow(10,7)+ 1;
}

int main(int argc, char* argv[]){
    std::ofstream csvFile;

    for (int k = 0; k <= 25000; k = k+2500)
    {
				csvFile.open("outside_i_time.csv", std::ios::app);
        int MAX_ARR_SIZE = k;

        double **A = (double **)malloc(MAX_ARR_SIZE * sizeof(double *));
        double *x = (double *)malloc(MAX_ARR_SIZE * sizeof(double));
        double *B = (double *)malloc(MAX_ARR_SIZE * sizeof(double));
        double aux = 0;

        double time_spent = 0.0;

        clock_t begin = clock();

        for (int i = 0; i < MAX_ARR_SIZE; i++)
        {
            A[i] = (double *)malloc(MAX_ARR_SIZE * sizeof(double));
            x[i] = getRandomNumber();
            for (int j = 0; j < MAX_ARR_SIZE; j++)
            {
                A[i][j] = getRandomNumber();
            }
        }

        for (int i = 0; i < MAX_ARR_SIZE; i++)
        {
            for (int j = 0; j < MAX_ARR_SIZE; j++)
            {
                B[i] += A[i][j] * x[j];
            }
        }
				std::cout<<k<<std::endl;
				for(int i =0; i< MAX_ARR_SIZE; i++){
					free(A[i]);
				}
        free(x);
        free(B);
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        csvFile<<k<<","<<time_spent<<",";
				csvFile.close();
    }
    return 0;
}
