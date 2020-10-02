#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

///--mpicxx multiMaVector.cpp -o vector
///--mpirun --mca btl_base_warn_component_unused 0 -np 4 vector

// Una función para leer y distribuir un vector.
void Read_vector(double local_a[], int local_n, int n, char vec_name, int my_rank, MPI_Comm comm){
  double* a = NULL;
  int i;

  if(my_rank == 0){
    a = (double*)malloc(n*sizeof(double));
    cout<< "Ingrese el vector "<< vec_name << endl; 
    for(i = 0; i < n; i++)
      {scanf("%lf", &a[i]);}
    MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE,0,comm);
    free(a);
  }else{
      MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
  }
}

//Una función para imprimir un vector distribuido
void Print_vector(double local_b[], int local_n, int n, char title, int my_rank, MPI_Comm comm){
  double* b = NULL;
  int i;
  if(my_rank == 0){
    b = (double*)malloc(n*sizeof(double));
    MPI_Gather(local_b, local_n, MPI_DOUBLE,b,local_n, MPI_DOUBLE,0,comm);
    cout<<title<<endl;
    for(i = 0; i < n; i++){
      printf("[%f] \n", b[i]);
    }
    printf("\n");
    free(b);
  } else{
    MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
  }
}

//Una función de multiplicación de matriz-vector MPI
void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n, int local_n, MPI_Comm comm){
  double* x;
  int local_i,j;
  int local_ok = 1;
  x = (double*)malloc(n*sizeof(double));
  MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);
  for(local_i = 0; local_i < local_m; local_i++){
    local_y[local_i] = 0.0;
    for(j = 0; j <n; j++){
      local_y[local_i] += local_A[local_i*n+j] * x[j];
    }
  }
  free(x);
}

int main(int argc,char **argv){
  int comm_sz;
  int my_rank;
  
  int m=4; ///filas
  int n=4; ///columnas
  int local_n,local_m;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  local_n = n/comm_sz; /*Divide Matriz en bloques  para los n procesos*/
  local_m = m/comm_sz;
  double local_x[local_n],local_y[local_m],local_A[local_n*local_m];
  Read_vector(local_A,local_n+local_m,n*m, 'A',my_rank,MPI_COMM_WORLD);
  Read_vector(local_x,local_n,n,'X',my_rank,MPI_COMM_WORLD);


  ///tomando el tiempo de ejecucion
  double tInicio,tFin;
  double MPI_Wtime(void);

  tInicio = MPI_Wtime();
  Mat_vect_mult(local_A, local_x, local_y, local_m,n,local_n,MPI_COMM_WORLD);
  tFin = MPI_Wtime();
  Print_vector(local_y,local_m,m,'Y', my_rank,MPI_COMM_WORLD);

  printf("El tiempo transcurrido es : %f  segundos \n ", tFin - tInicio);

  
  MPI_Finalize();
  return 0;
}
