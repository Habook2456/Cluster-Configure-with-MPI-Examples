#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Mat_vect_mult(double local_A[], /* in */
                   double local_x[], /* in */
                   double local_y[], /* out */
                   int local_m,       /* in */
                   int n,             /* in */
                   int local_n,       /* in */
                   MPI_Comm comm)     /* in */ {
    double* x;
    int local_i, j;

    x = (double*) malloc(n * sizeof(double));

    MPI_Allgather(local_x, local_n, MPI_DOUBLE,
                  x, local_n, MPI_DOUBLE, comm);

    for (local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++) {
            local_y[local_i] += local_A[local_i * n + j] * x[j];
        }
    }

    free(x);
}

int main(int argc, char* argv[]) {
    int comm_sz; 
    int my_rank; 
    MPI_Comm comm;
    int m = 8, n = 8; 
    double* local_A;   
    double* local_x;   
    double* local_y;   
    int local_m, local_n;

    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    comm = MPI_COMM_WORLD;

    local_m = m / comm_sz;  
    local_n = n / comm_sz;  

    
    srand(time(NULL) + my_rank);

    
    local_A = (double*) malloc(local_m * n * sizeof(double));
    local_x = (double*) malloc(local_n * sizeof(double));
    local_y = (double*) malloc(local_m * sizeof(double));

    
    for (int i = 0; i < local_m * n; i++) {
        local_A[i] = (double)rand() / RAND_MAX;
    }
    for (int i = 0; i < local_n; i++) {
        local_x[i] = (double)rand() / RAND_MAX;
    }

    
    printf("Proceso %d: Tamaño de la submatriz local: %d x %d\n", my_rank, local_m, n);
    printf("Proceso %d: Tamaño del vector local: %d\n", my_rank, local_n);

    
    Mat_vect_mult(local_A, local_x, local_y, local_m, n, local_n, comm);

    
    printf("Proceso %d: Tamaño del resultado local: %d\n", my_rank, local_m);

    
    printf("Proceso %d: Valores de la submatriz local:\n", my_rank);
    for (int i = 0; i < local_m * n; i++) {
        printf("%f ", local_A[i]);
    }
    printf("\n");

    printf("Proceso %d: Valores del vector local:\n", my_rank);
    for (int i = 0; i < local_n; i++) {
        printf("%f ", local_x[i]);
    }
    printf("\n");

    printf("Proceso %d: Resultados locales de la multiplicación matriz-vector:\n", my_rank);
    for (int i = 0; i < local_m; i++) {
        printf("%f ", local_y[i]);
    }
    printf("\n");

    
    free(local_A);
    free(local_x);
    free(local_y);

    MPI_Finalize();
    return 0;
}
