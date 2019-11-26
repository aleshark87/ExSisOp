/*defining macros*/
#define TIPO_A 0
#define TIPO_B 1
#define numPRODa 3
#define numPRODb 5
#define numCONS 10
#define SLEEP_TIME 1
#define NUMBUFFER 1

/*library inclusion*/
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 

uint64_t datoA = 0;
uint64_t datoB = 0; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condWaitA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWaitB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCons = PTHREAD_COND_INITIALIZER;
pthread_cond_t aspettoBro = PTHREAD_COND_INITIALIZER;
int bufferA = 0;
int bufferB = 0;

void* produttoreA(void *arg){
	while(1){
		sleep(1);
		pthread_mutex_lock(& mutex);
		while(bufferA >= NUMBUFFER){
			pthread_cond_wait(& condWaitA, & mutex);
		}
		printf("PRODOTTO DATO A \n");
		datoA++;
		bufferA++;
		pthread_cond_signal(&condCons);
		pthread_mutex_unlock(& mutex);
	}
}

void* produttoreB(void *arg){
	while(1){
		sleep(1);
		pthread_mutex_lock(& mutex);
		while(bufferB >= NUMBUFFER){
			pthread_cond_wait(& condWaitB, & mutex);
		}
		printf("PRODOTTO DATO B \n");
		datoB++;
		bufferB++ ;
		pthread_cond_signal(&condCons);
		pthread_mutex_unlock(& mutex);
	}
}

void* consumatoreAB(void *arg){
	while(1){
		pthread_mutex_lock(& mutex);
		while((bufferA <= 0) || (bufferB <= 0)){
			pthread_cond_wait(&condCons, &mutex);
		}
		datoA--;
		datoB--;
		bufferA--;
		bufferB--;
		printf("CONSUMATO DATO A e B \n");
		pthread_cond_signal(&condWaitA);
		pthread_cond_signal(&condWaitB);
		pthread_mutex_unlock(& mutex);
		sleep(1);
	}
}

int main(void){
	pthread_t tid;
	int i;
	for(i = 0; i < numPRODa; i++){
		pthread_create(& tid , NULL, produttoreA, NULL);
	}
	for(i = 0; i < numPRODb; i++){
		pthread_create(& tid, NULL, produttoreB, NULL);
	}
	for(i = 0; i < numCONS; i++){
		pthread_create(& tid, NULL, consumatoreAB, NULL);
	}
	pthread_exit(NULL);
}


