#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(){
    //initailize the mpi with defult configuration
    MPI_Init(NULL, NULL);
    //the matrix representing the linear expressions
    double **M;
    //row number of the matrix, and the colume number should be one more than row number
    int rank = 500;//large enough size to show efficiency
    //pid is the unique id for each processor, pnum is total number of processors
    int pid, pnum;
    //pointers that will be used
    int i, j, r;
    //ratio of ith row and rows dowmn to it
    double ratio ;
    //the timer variable
    double dtime ;

    //to set up values for pid and pnum 
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pnum);


    //create an empty 2D matrix by dynamicly allocating memory;
    //allocate memory for first dimension
    M = (double**)malloc(sizeof(double*)*rank);
    //allocate memory for second dimension
    for (i = 0; i < rank; i++){
        M[i] = (double*)malloc(sizeof(double)*(rank + 1));
    }

    //generate a rank*rank+1 matrix with random elements
    //make the generation operation only for root(id is 0) processor
    if (pid == 0){
        for (i = 0; i < rank; i++){
            for (j = 0; j < rank + 1; j++){
                // generate random values within 100
                M[i][j] = rand() % 100;
            }
        }
    /*//this pattern is to test correctness of the algorithm.
        double test[30] = {2, 1, 2, 2, 1, 23, -1, 0, 1, 2, 1, 15, 0, 1, -1, 2, 0, 7, 1, 2, 0, 1, 0, 9, 1, 0, 1, 0, 1, 9};
        int k = 0;
        for (i = 0; i < rank; i++){
            for (j = 0; j < rank + 1; j++){
                M[i][j] = test[k];
                k++;
            }
        }
    */
    }
    //start the timer
    dtime = MPI_Wtime();

    //broadcast to all processors
    for (i = 0; i < rank; i++){
        MPI_Bcast(&M[i][0], rank + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    //this is a way of static allocating, to allocate almost same amounts of rows to each processor in advance of operation
    int *alocs = malloc(sizeof(int)*rank);
    //it means ith row is taken by processor whose id is alocs[i]
    for (i = 0; i < rank; i++){
        alocs[i] = i % pnum;
    }

    //make sure every processor is ready before elimination, may be unnecessary, but I like it :)
    MPI_Barrier(MPI_COMM_WORLD);

    //GaussianElimination();
    for (i = 0; i < rank - 1; i++){
        //before row reductions, broadcast the current row by the processor who was reponsible to handle it
        //because this row is currently inivisible to other processors, while their operations are relative to is values
        MPI_Bcast (&M[i][i], rank - i + 1, MPI_DOUBLE, alocs[i], MPI_COMM_WORLD);
        for (j = i + 1; j < rank; j++){
            //pick the processor who was allocated to the jth row to handle reduction
            if (alocs[j] == pid){
                ratio = M[j][i] / M[i][i];
                for (r = i; r < rank + 1; r++){
                    M[j][r] -= ratio * M[i][r];
                }
            }
        }
    }
    //broadcast again at the end, because the last row has not been broadcast as i only loop to rank - 1
    MPI_Bcast (&M[rank - 1][rank - 1], 2, MPI_DOUBLE, alocs[rank - 1], MPI_COMM_WORLD);

    //make sure every thing is done before the next step
    MPI_Barrier(MPI_COMM_WORLD);

    //resolve();
    //i set the resolve operation only for root processor the handle
    if (pid == 0){
        //firstly figure out the last variable's value
        M[rank-1][rank] /= M[rank-1][rank-1];

        for (i = rank - 2; i >= 0; i--){
            for (j = i + 1; j < rank; j++){
                M[i][rank] -= M[j][rank] * M[i][j];
            }
            M[i][rank] /= M[i][i];
        }
    }
    
    //end timer
    dtime = MPI_Wtime() - dtime;
    //also only root processor for printing task 
    if (pid == 0){
        printf("\n time used: %f ms \n", dtime*1000);
        //for reading the time more easily, i have to comment out the answers printing
    /*  printf("answers: \n");
        for (i = 0; i < rank; i++){
            printf("%f\t", M[i][rank]);
        }*/
    }

    MPI_Finalize();
    return 0;
}
