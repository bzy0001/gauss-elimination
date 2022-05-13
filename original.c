#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
/*this is gauss elimination file without omp or mpi, just for comparing */

//the matrix representing the linear expressions
double **M;
//row number of the matrix, and the colume number should be one more than row number
int rank;

void generateMatrix(){
    int i, j;
    //create an empty 2D matrix;
    M = (double**)malloc(sizeof(double*)*rank);
    for (i = 0; i < rank; i++){
        M[i] = (double*)malloc(sizeof(double)*(rank + 1));
    }
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            // generate random values within 100
            M[i][j] = rand() % 100;
        }
    }
    //this pattern is to test correctness of the algorithm.
  /*  double test[30] = {2, 1, 2, 2, 1, 23, -1, 0, 1, 2, 1, 15, 0, 1, -1, 2, 0, 7, 1, 2, 0, 1, 0, 9, 1, 0, 1, 0, 1, 9};
    int k = 0;
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            M[i][j] = test[k];
            k++;
        }
    }*/
}

void GaussianElimination(){
    double ratio = 0;
    int i, j, r;

    for (i = 0; i < rank - 1; i++){
        //one time make one colume zero (down from row i)
        for (j = i + 1; j < rank; j++){
            ratio = M[j][i] / M[i][i];
            for (r = i; r < rank + 1; r++){
                M[j][r] -= ratio * M[i][r];
            }
        }
    }
}

void resolve(){
    int i, j;
    //firstly figure out the last variable's value
    M[rank-1][rank] /= M[rank-1][rank-1];

    for (i = rank - 2; i >= 0; i--){
        for (j = i + 1; j < rank; j++){
            M[i][rank] -= M[j][rank] * M[i][j];
        }

        M[i][rank] /= M[i][i];
    }
}



int main(){
    rank = 500;
    int i;

    generateMatrix();

    double dtime = omp_get_wtime();

    GaussianElimination();

    resolve();

    dtime = omp_get_wtime() - dtime;

    //for reading the time more easily, i have to comment out the answers printing
    /*printf("answers: \n");
    for (i = 0; i < rank; i++){
        printf("%f\t", M[i][rank]);
    }*/
    printf("\n time uesd: %f ms \n", dtime*1000);
    return 0;
}