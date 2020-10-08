/* File:     poduc-consumidor.c
 *
 * Proposito:  Este es un problema de sincronización con pthreads: 
 *              crea algunos hilos, cada uno de los cuales crea e imprime un mensaje.
 *
 *
 * Section:  Section 4.7 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int MAX_THREADS = 1024; //variable para hilos
const int MSG_MAX = 100;

/* Variables Globales:  accessible to all threads */
int thread_count;// para contar los hilos
char** messages; //vector

void Usage(char* prog_name);
void *Send_msg(void* rank); /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	long thread;
	pthread_t* thread_handles; //utilizamos para un espacio de memoria

	if (argc != 2)//contenedor de hilos
		Usage(argv[0]); // funcion para inprimir un mensaje 
	thread_count = strtol(argv[1], NULL, 10);
	if (thread_count <= 0 || thread_count > MAX_THREADS)// condicion de los hilos no se tiene que pasar sobre la cantidad de hilos que tenmos(1024) ni ser negativo
		Usage(argv[0]);

	thread_handles = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	messages = (char**) malloc(thread_count * sizeof(char*)); // Reserva un espacio de memoria(thread_count) con la dimension de hilos
	for (thread = 0; thread < thread_count; thread++)  
		messages[thread] = NULL;// en el arreglo mensajes, guardamos en thread los valores con null 

	for (thread = 0; thread < thread_count; thread++)// inicializamos nuestro hilo
		pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL,
				Send_msg, (void*) thread);

	for (thread = 0; thread < thread_count; thread++) { //junta los hilos
		pthread_join(thread_handles[thread], NULL);
	}

	for (thread = 0; thread < thread_count; thread++)// liberamos espacio de mensajes y espacio de memoria
		free(messages[thread]);
	free(messages);

	free(thread_handles);
	return 0;
} /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

	fprintf(stderr, "usage: %s <numero de hilos>\n", prog_name);
	exit(0);
} /* Usage */

/*-------------------------------------------------------------------
 * Function:       Send_msg
 * Purpose:        Create a message and ``send'' it by copying it
 *                 into the global messages array.  Receive a message
 *                 and print it.
 * In arg:         rank
 * Global in:      thread_count
 * Global in/out:  messages
 * Return val:     Ignored
 * Note:           The my_msg buffer is freed in main
 */
void *Send_msg(void* rank) {
	long my_rank = (long) rank;
	long dest = (my_rank + 1) % thread_count; // sacamos el modulo para la variable destino (dest)
	long source = (my_rank + thread_count - 1) % thread_count;
	char* my_msg = (char*) malloc(MSG_MAX * sizeof(char));

	sprintf(my_msg, "Hola del thread %ld al thread %ld", dest, my_rank);
	messages[dest] = my_msg;

	if (messages[my_rank] != NULL)
		printf("Thread %ld > %s\n", my_rank, messages[my_rank]);
	else
		printf("Thread %ld > No hay mensaje de %ld\n", my_rank, source);

	return NULL;
} /* Send_msg */
