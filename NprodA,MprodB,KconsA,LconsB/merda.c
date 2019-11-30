/*defining macros*/
#define numPRODa 4
#define numPRODb 5
#define numCONSa 6
#define numCONSb 4
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
uint64_t buf = 0;
uint64_t numBufAPieni = 0;
uint64_t numBufBPieni = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condProdA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condProdB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsB = PTHREAD_COND_INITIALIZER;

void* produttoreA(void *arg){
	while(1){
		/* qua VIENE PRODOTTO il dato */
		sleep(SLEEP_TIME);
		datoA++;
		printf("PRODUCO DATO A \n");
		pthread_mutex_lock(&mutex);
		while(numBufAPieni == NUMBUFFER){
			printf("SONO A e ASPETTO \n");
			pthread_cond_wait(&condProdA, &mutex);
		}
		buf = datoA;
		datoA--;
		numBufAPieni++;
		pthread_cond_signal(&condConsA);
		printf("CONS A SIGNALLED \n");
		pthread_mutex_unlock(&mutex);
	}
}

void* produttoreB(void *arg){
	while(1){
		/* qua VIENE PRODOTTO il dato */
		sleep(SLEEP_TIME);
		datoB++;
		printf("PRODUCO DATO B \n");
		pthread_mutex_lock(&mutex);
		while(numBufBPieni == NUMBUFFER){
			printf("SONO B e ASPETTO \n");
			pthread_cond_wait(&condProdB, &mutex);
		}
		buf = datoB;
		datoB--;
		numBufBPieni++;
		pthread_cond_signal(&condConsB);
		pthread_mutex_unlock(&mutex);
	}
}

void* consumatoreA(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(numBufAPieni == 0){
			pthread_cond_wait(&condProdA,&mutex);
		}
		/* CONSUMO */
		numBufAPieni--;
		pthread_cond_signal(&condProdA);
		pthread_mutex_unlock(&mutex);
		printf("CONSUMO A \n");
		sleep(SLEEP_TIME);
	}
}

void* consumatoreB(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(numBufBPieni == 0){
			pthread_cond_wait(&condProdB,&mutex);
		}
		/* CONSUMO */
		numBufBPieni--;
		pthread_cond_signal(&condProdB);
		pthread_mutex_unlock(&mutex);
		printf("CONSUMO B \n");
		sleep(SLEEP_TIME);
	}
}

int main(void){
	int i ;
	pthread_t tid;
	for(i = 0; i < numPRODa; i++){
		pthread_create(&tid, NULL, produttoreA, NULL);
	}
	for(i = 0; i < numPRODb; i++){
		pthread_create(&tid, NULL, produttoreB, NULL);
	}
	for(i = 0; i < numCONSa; i++){
		pthread_create(&tid, NULL, consumatoreA, NULL);
	}
	for(i = 0; i < numCONSb; i++){
		pthread_create(&tid, NULL, consumatoreB, NULL);
	}
	pthread_exit(NULL);
}
