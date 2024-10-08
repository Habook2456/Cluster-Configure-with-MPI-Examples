#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double f(double x) {
    return x * x; 
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
    double estimate, x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++) {
        x = left_endpt + i * base_len;
        estimate += f(x);
    }
    estimate = estimate * base_len;

    return estimate;
}

int main(void) {
    int my_rank, comm_sz, n, local_n;
    double a, b, h, local_a, local_b;
    double local_int, total_int = 0.0;
    int source;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        // Solo el proceso 0 solicita la entrada
        printf("Ingrese el límite inferior (a): ");
        scanf("%lf", &a);
        printf("Ingrese el límite superior (b): ");
        scanf("%lf", &b);
        printf("Ingrese el número de trapecios (n): ");
        scanf("%d", &n);
    }

    
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = (b - a) / n;  
    local_n = n / comm_sz;  

    local_a = a + my_rank * local_n * h;  
    local_b = local_a + local_n * h;  
    local_int = Trap(local_a, local_b, local_n, h); 

    
    if (my_rank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_int = local_int;  
        for (source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
        
        printf("Con n = %d trapecios, nuestra estimación\n", n);
        printf("de la integral desde %f hasta %f = %.15e\n", a, b, total_int);
    }

    MPI_Finalize();
    return 0;
}
