/* File:  
 *    pth_barre_mutex.c
 *
 * Proposito:
 *   Utilizar barreras de espera ocupada para sincronizar hilos.
 *
 * Input:
 *    none
 * Output:
 *    Tiempo para barreras BARRIER_COUNT
 *
 * Compilar:
 *    mpicc -o barrera-mutex-750 pth_barre_mutex.c -lpthread
 *
 * Uso:
 *    mpirun -np 1 barrera-mutex-750 <numero de hilos>
 *
 * Nota:
 *    Compilar flag DEBUG imprimira un mensaje despues de cada barrera   
 *
 * IPP:   Section 4.8.1 (p. 177)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define BARRIER_COUNT 750

int thread_count;
int barrier_thread_counts[BARRIER_COUNT];
pthread_mutex_t barrier_mutex;  //inicializa el mutex con los atributos por omisión. Es equivalente a invocar:
                                //pthread_mutex_t mylock;
                                //pthread_mutex_init(& mylock, NULL);

void Usage(char* prog_name);
void *Thread_work(void* rank);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread, i;
   pthread_t* thread_handles; 
   double start, finish;

   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   for (i = 0; i < BARRIER_COUNT; i++)
      barrier_thread_counts[i] = 0;
   pthread_mutex_init(&barrier_mutex, NULL); //permite dar las condiciones iniciales a un candado mutex

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   GET_TIME(finish);
   printf("Elapsed time = %e seconds\n", finish - start);

   pthread_mutex_destroy(&barrier_mutex); // Destruye la variable (de tipo pthread_mutex_t) usada para manejo de explusión mutua, o candado mutes
   free(thread_handles);
   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Funcion  : Usage
 * Proposito: Imprimir línea de comando para función y terminar
 * In arg   : prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <numero de hilos>\n", prog_name);
   exit(0);
}  /* Usage */


/*-------------------------------------------------------------------
 * Funcion:    Thread_work
 * Proposito:    Correr barreras BARRIER_COUNT 
 * In arg:      rank
 * Global var:  thread_count, barrier_thread_counts, barrier_mutex
 * Return val:  Ignorado
 */
void *Thread_work(void* rank) {
#  ifdef DEBUG
   long my_rank = (long) rank; 
#  endif
   int i;

   for (i = 0; i < BARRIER_COUNT; i++) {
      pthread_mutex_lock(&barrier_mutex);// Permite solicitar acceso al mutex, el hilo se bloquea hasata su obtención
      /* Sección crítica */
      barrier_thread_counts[i]++;
      pthread_mutex_unlock(&barrier_mutex); // Permute liberar un mutex.
      while (barrier_thread_counts[i] < thread_count);
   }

   return NULL;
}  /* Thread_work */
