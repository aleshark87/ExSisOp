#define N 10
#define M 20
#define K 2
#define L 1
#define NUMBUFFER 1

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <pthread.h> 

int buffer = 0;
uint64_t datoA = 0;
uint64_t datoB = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condWaitA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWaitB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsB = PTHREAD_COND_INITIALIZER;

void* produttoreA(void* arg){
	int indice;
	indice = * (int*) arg; indice++;
	while(1){
		printf("PRODUZIONE DATO A, sono il %d\n", indice);
		sleep(3);
		pthread_mutex_lock(&mutex);
		while(buffer >= NUMBUFFER){
			pthread_cond_wait(&condWaitA, &mutex);
		}
		printf("METTO NEL BUFFER IL DATO A\n");
		datoA++; buffer = datoA; datoA--;
		pthread_cond_signal(&condConsA);
		pthread_mutex_unlock(&mutex);
	}
}

void* produttoreB(void* arg){
	int indice;
	indice = * (int*) arg; indice++;
	while(1){
		printf("PRODUZIONE DATO B, sono il %d\n", indice);
		sleep(3);
		pthread_mutex_lock(&mutex);
		while(buffer >= NUMBUFFER){
			pthread_cond_wait(&condWaitB, &mutex);
		}
		printf("METTO NEL BUFFER IL DATO B\n");
		datoA++; buffer = datoA; datoA--;
		pthread_cond_signal(&condConsB);
		pthread_mutex_unlock(&mutex);
	}
}

void* consumatoreA(void* arg){
	int indice;
	indice = * (int*) arg; indice++;
	while(1){
		pthread_mutex_lock(&mutex);
		while(buffer == 0){
			pthread_cond_wait(&condConsA, &mutex);
		}
		buffer--;
		printf("CONSUMATO DATO A.., sono il %d\n", indice);
		pthread_cond_signal(&condWaitA);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}
}

void* consumatoreB(void* arg){
	int indice;
	indice = * (int*) arg; indice++;
	while(1){
		pthread_mutex_lock(&mutex);
		while(buffer == 0){
			pthread_cond_wait(&condConsB, &mutex);
		}
		buffer--;
		printf("CONSUMATO DATO B.., sono il %d\n", indice);
		pthread_cond_signal(&condWaitB);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}
}

int main(void){
	pthread_t tid[N + M + K + L];
	int indice; int *p;
	
	for(indice = 0; indice < N; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, produttoreA, (void*)p);
	}
	
	for(indice = N; indice < (N + M); indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, produttoreB, (void*)p);
	}
	
	for(indice = (N + M); indice < (N + M + K); indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, consumatoreA, (void*)p);
	}
	
	for(indice = (N + M + K); indice < (N + M + K + L); indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, consumatoreB, (void*)p);
	}
	pthread_exit(NULL);
}	
