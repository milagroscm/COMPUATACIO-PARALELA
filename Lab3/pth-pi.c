/*
* Propósito: Estimar pi usando series
*       pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]
* Una seccion con Busy-waiting y otra con mutex 
* 
*/

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

long double sum;
long flag;
int numThreads;
int n;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_sum_bs(void * rank){
	long my_rank = *((long*) rank);
	long double factor;
	long double local_n = n / numThreads;
	long double my_first_i = local_n * my_rank;
	long double my_last_i = my_first_i + local_n;
	long double my_sum = 0.0;
	if((int) my_first_i % 2 == 0){
		factor = 1.0;
	}
	else{
		factor = -1.0;
	}

	for(long double i = my_first_i; i < my_last_i; i++, factor = -factor){
		my_sum += factor/(2 * i + 1);
	}
    
    while(flag != my_rank);
    sum += my_sum;
	flag = (flag+1) % numThreads; 

	return NULL;
}

/*    mutex     */

void * thread_sum_mutex(void * rank){
	long my_rank = *((long*) rank);
	long double factor;
	long double local_n = n / numThreads;
	long double my_first_i = local_n * my_rank;
	long double my_last_i = my_first_i + local_n;
	long double my_sum = 0.0;
	if((int) my_first_i % 2 == 0){
		factor = 1.0;
	}
	else{
		factor = -1.0;
	}

	for(long double i = my_first_i; i < my_last_i; i++, factor = -factor){
		my_sum += factor/(2 * i + 1);
	}
    
    while(flag != my_rank);
	sum += my_sum;
	flag = (flag+1) % numThreads; 

	pthread_mutex_lock(&mutex);
	sum += my_sum;
	pthread_mutex_unlock(&mutex);

    return NULL;	
}

int main(int argc, char ** argv){
	if(argc != 3){
		printf("Faltan argumentos <numThread> <N>\n");
		return 0;
	}
	numThreads = atoi(argv[1]);
	n = atoi(argv[2]);
	pthread_t threads[numThreads];
	long threads_args[numThreads];
	sum = 0;
	flag = 0;
	double start,end;
	GET_TIME(start);
	for(long i = 0; i < numThreads; i++){
		threads_args[i] = i;
		pthread_create(&threads[i], NULL, thread_sum_mutex, &threads_args[i]);
	}
	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}
	GET_TIME(end);
	sum = 4 * sum;
	double seconds = end - start;
	printf("%Lf\n",sum);
	printf("Tiempo->%lf\n",seconds);
}
