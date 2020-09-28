#include <iostream>
#include <string.h>
#include <mpi.h>

using namespace std;
//mpicc main.c
//mpiexec -n 8 ./a.out 

double f(double x)
{
    return x*x;
}

double Trap(double Dato_A, double Dato_B, int trap_count, double base_len)
{
    double approx, x;
    int i;

    approx = (f(Dato_A) + f(Dato_B)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++)
    {
        x = Dato_A + i * base_len;
        approx += f(x);
    }
    approx = approx * base_len;

    return approx;
}

int main()
{
    int my_rank, comm_sz, n = 1024, local_n;
    double a = 0.0, b = 3.0, h, local_a, local_b;
    double local_integral, total_integral;
    int proc;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    //imprime en consola el nucleo que ejecuta  el proceso
    cout<<"soy el core nro: "<< my_rank<< " de "<< comm_sz<< endl;  

    //h y locla_n son iguales para todos los procesos
    h = (b - a)/n;          // longitud de cada trapecio
    local_n = n/comm_sz;    //cantidad de trapecios por proceso

    //Duración del intervalo de integración de cada proceso = local_n * h.
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    local_integral = Trap(local_a, local_b, local_n, h);

    if (my_rank != 0) {
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_integral = local_integral;
        for (proc = 1; proc < comm_sz; proc++) {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += local_integral;
        }
    }
    if (my_rank == 0) {
      cout<< "Con n = " << n<< " trapezoides"<<endl;
      cout<< "El valor de la integral entre "<< a << " y " << b << " = " <<total_integral<<endl;

    }
    MPI_Finalize();
    return 0;
}
