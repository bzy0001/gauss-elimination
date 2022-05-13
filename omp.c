
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


//the matrix representing the linear expressions
double **M;
//row number of the matrix, and the colume number should be one more than row number
int rank;

void generateMatrix(){
    int i, j;
    //create an empty 2D matrix by dynamicly allocating memory;
    //allocate memory for first dimension
    M = (double**)malloc(sizeof(double*)*rank);
    //allocate memory for second dimension
    for (i = 0; i < rank; i++){
        M[i] = (double*)malloc(sizeof(double)*(rank + 1));
    }
    //generate a rank*rank+1 matrix with random elements
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            // generate random values within 100
            M[i][j] = rand() % 100;
        }
    }

/*    //this pattern is to test correctness of the algorithm. 
    double test[30] = {2, 1, 2, 2, 1, 23, -1, 0, 1, 2, 1, 15, 0, 1, -1, 2, 0, 7, 1, 2, 0, 1, 0, 9, 1, 0, 1, 0, 1, 9};
    int k = 0;
    printf("type: ");
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            M[i][j] = test[k];
            k++;
        }
    }
*/
}

//gaussian elimination, omp implemented
void GaussianElimination(){
    //ratio of ith row and rows dowmn to it
    double ratio = 0;
    int i, j, r;
    for (i = 0; i < rank - 1; i++){
        //implementing omp parallel for, with shared and private variables, 
        //autometicly allocate for loops to each thread
        #pragma omp parallel for shared(rank, M, i) private(ratio, j, r)
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
    //every time make right side value deducted by product of other parameters and thier right side value
    for (i = rank - 2; i >= 0; i--){
        for (j = i + 1; j < rank; j++){
            M[i][rank] -= M[j][rank] * M[i][j];
        }
        //divide the value by parameter, to get one result
        M[i][rank] /= M[i][i];
    }

}
int main(){
    //large enough size to show efficiency
    rank = 500;
    int i;
    
    //generate a matrix
    generateMatrix();

    //start the timer
    double dtime = omp_get_wtime();

    //start to form a row echelon form
    GaussianElimination();
    //wait until row echelon form is formed
    #pragma omp barrier
    //resolve answers down to up
    resolve();

  /*//for reading the time more easily, i have to comment out the answers printing
    printf("answers: \n");
    for (i = 0; i < rank; i++){
        printf("%f\t", M[i][rank]);
    }*/

    //figure out the time used
    dtime = omp_get_wtime() - dtime;
    printf("\n time uesd: %f ms \n", dtime*1000);

    return 0;
}
