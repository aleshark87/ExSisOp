#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

int turn = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* ThreadFunction(void* arg){
	intptr_t myindex ;
	myindex = (intptr_t) arg;
	while(1){
		pthread_mutex_lock(& mutex);
		/*quando non è il mio turno*/
		while(turn != myindex){
			/*aspetto, e sto pure zitto*/
			printf("sono thread %" PRIdPTR "\n",myindex);
			sleep(2);
			pthread_cond_wait(& cond, & mutex);
			if(turn != myindex){
				pthread_cond_signal(& cond);
			}
		}
		/*SEZIONE CRITICA*/
		turn = (turn + 1) % 3; /*turno del successivo*/
		pthread_cond_signal(& cond); /*risveglio qualcuno*/
		pthread_mutex_unlock(& mutex);
	}
}

int main(void){
	intptr_t i;
	pthread_t tid;
	for(i = 0; i < 3; i++){
		pthread_create(&tid, NULL, ThreadFunction, (void*)i);
	}
	pthread_exit(NULL);
}
