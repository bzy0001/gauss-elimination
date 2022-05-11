#include<stdio.h>
#include<stdlib.h>

//the matrix representing the linear expressions
double **M; 
//row number of the matrix, and the colume number should be one more than row number
unsigned int rank; 
//counters that will be used
int i, j, r;

void generateMatrix(){   
    printf("please enter the row number of the matrix (an integer larger than 1): ");
    //get the size of the matrix
    scanf("%u", &rank); 

    //create an empty 2D matrix;
    M = (double**)malloc(sizeof(double*)*rank); 
    for (i = 0; i < rank; i++){
        M[i] = (double*)malloc(sizeof(double)*(rank + 1));
    }
    printf("please enter elements from left to right and up to down, with one space between two elements.\n");
    //read each element into the matrix
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            scanf("%lf", &M[i][j]); 
        }
    }
}

void GaussianElimination(){
    double ratio = 0;
    for (i = 0; i < rank - 1; i++){
        //one time make one colume zero (down from row i)
        for (j = i + 1; j < rank; j++){
            ratio = M[j][i] / M[i][i];
            //substract the whole row
            for (r = i; r < rank + 1; r++){
                M[j][r] -= ratio * M[i][r];
            }
        }
    }
}

void resolve(){
    //firstly figure out the last variable's value
    M[rank-1][rank] /= M[rank-1][rank-1];
    for (i = rank - 2; i >= 0; i--){
        //substract sum of other varaibles multiply their parameters
        for (r = i + 1; r < rank; r++){
                M[i][rank] -= M[r][rank] * M[i][r];               
            }
        //get value of this variable by dividing its parameter
        M[i][rank] /= M[i][i];
    }
}

//print matrix before and after excution, for more readability of output
void printMatrix(){
    for (i = 0; i < rank; i++){
        for (j = 0; j < rank + 1; j++){
            printf("%f\t", M[i][j]); 
        }
        printf("\n");
    }
}

int main(){
    generateMatrix();
    printf("original matrix:\n");
    printMatrix();

    GaussianElimination();
    printf("row echelon form after gaussian elimination:\n");
    printMatrix();

    resolve();
    printf("answers: \n");
    for (i = 0; i < rank; i++){
        printf("%f\t", M[i][rank]);
    }
    return 0;
}
