#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
  const int n_operaciones = 10;


  MPI_Init(NULL, NULL);
  int rank_actual;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_actual);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int contador = 0;
  int rank = (rank_actual + 1) % 2;
  while (contador < n_operaciones) {
    if (rank_actual == contador % 2) {
      contador++;
      MPI_Send(&contador, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
      sleep(1);//0.5 seg
      cout<<"Proceso "<< rank_actual<< " envio e incremento contador a "<< contador<< " al proceso "<< rank<< endl;
      sleep(1);//0.5 seg
    } else {
      MPI_Recv(&contador, 1, MPI_INT, rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      sleep(1);//0.5 seg
      cout<<"Proceso "<< rank_actual<<" recibio contador "<< contador<< " de proceso " << rank<<endl;
      sleep(1);//0.5 seg
    }
  }
  MPI_Finalize();
}
